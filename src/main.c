#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "midi.h"
#include "writer.h"
#include "debug.h"

int main() {
  // SMF Data
  SMF smf;
  SMF_init(&smf);

  TrackEvent tempo_ev;
  MetaEvent_init_tempo(&tempo_ev, (uint32_t)0, smf.tempo);
  debug_printf("delta: %d", tempo_ev.delta);
  MTrack_append(&smf.track, &tempo_ev);
  TrackEvent ts_ev;
  MetaEvent_init_time_signature(&ts_ev, (uint32_t)0, &smf.ts);
  MTrack_append(&smf.track, &ts_ev);
  TrackEvent melody[72];
  for (int i=0; i<36; i++) {
    MidiEvent_init(&melody[i*2], 0, MidiMsgType_NOTE_ON, 0x30 + i*2, 100, 0);
    MTrack_append(&smf.track, &melody[i*2]);
    MidiEvent_init(&melody[i*2+1], 96, MidiMsgType_NOTE_OFF, 0x30 + i*2, 0, 0);
    MTrack_append(&smf.track, &melody[i*2+1]);
  }
  TrackEvent end_ev;
  MetaEvent_init_endoftrack(&end_ev, 0);
  debug_printf("end_ev.len: %d", end_ev.meta.len);
  MTrack_append(&smf.track, &end_ev);

  // Save
  Writer writer;
  Writer_open(&writer, "output/sample.mid");
  Writer_write_smf(&writer, &smf);
  Writer_close(&writer);

  SMF_release(&smf);
  return 0;
}
