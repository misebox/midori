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
  TrackEvent ts_ev;
  TrackEvent end_ev;
  MetaEvent_init_tempo(&tempo_ev, (uint32_t)0, smf.tempo);
  debug_print("delta: %d\n", tempo_ev.delta);
  MetaEvent_init_time_signature(&ts_ev, (uint32_t)0, &smf.ts);
  MetaEvent_init_endoftrack(&end_ev, 100);
  MTrack_append(&smf.track, &tempo_ev);
  MTrack_append(&smf.track, &ts_ev);
  MTrack_append(&smf.track, &end_ev);

  // Save
  Writer writer;
  Writer_open(&writer, "output/sample.mid");
  Writer_smf(&writer, &smf);
  Writer_close(&writer);

  SMF_release(&smf);
  return 0;
}
