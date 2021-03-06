#include "parse.h"
#include "vec.h"
#include "midi.h"
#include "debug.h"


void parse(SMF *smf, Vec *v) {
  // SMF Data
  TrackEvent *tempo_ev;
  MetaEvent_init_tempo(tempo_ev, (uint32_t)0, smf->tempo);
  debug_printf("delta: %d", tempo_ev->delta);
  MTrack_append(&smf->track, tempo_ev);
  TrackEvent *ts_ev;
  MetaEvent_init_time_signature(ts_ev, (uint32_t)0, &smf->ts);
  MTrack_append(&smf->track, ts_ev);
  TrackEvent *melody = (TrackEvent *)malloc(sizeof(TrackEvent));
  for (int i=0; i<61; i++) {
    MidiEvent_init(&melody[i*2], 0, MidiMsgType_NOTE_ON, 0x24 + i, 100, 0);
    MTrack_append(&smf->track, &melody[i*2]);
    MidiEvent_init(&melody[i*2+1], 120, MidiMsgType_NOTE_OFF, 0x30 + i, 0, 0);
    MTrack_append(&smf->track, &melody[i*2+1]);
  }
  TrackEvent *end_ev;
  MetaEvent_init_endoftrack(end_ev, 0);
  debug_printf("end_ev.len: %d", end_ev->meta.len);
  MTrack_append(&smf->track, end_ev);
  return;
}