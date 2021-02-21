#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "midi.h"

void VLUint_set(VLUint *vlu, uint32_t value) {
  assert(value < 0x10000000);
  int cur = 0;
  for (int i=0; i<=3; i++) {
    uint8_t byte = value >> (7 * (3 - i)) & 0xFF;
    if (cur > 0 || byte > 0 || i == 3) {
      vlu->bytes[cur] = byte;
      cur++;
    }
  }
  if (cur > 1) {
    vlu->bytes[0] |= 0x80;
  }
  vlu->size = cur;
}

// SMF
void TimeSignature_init(TimeSignature *ts) {
  ts->numer = 4;
  ts->denom = 4;
  ts->num_clocks = 24;
  ts->num_32nd = 8;
}
void SMF_init(SMF *smf) {
  MHead_init(&smf->head);
  MTrack_init(&smf->track, 10);
  smf->tempo = 120;
  TimeSignature_init(&smf->ts);
}
void SMF_release(SMF *smf) {
  MTrack_release(&smf->track);
}
// MHead
void MHead_init(MHead *head) {
  // TODO: there is no support for format1.
  head->format = 0;
  head->trackcount = 1;
  head->resolution = 480;
}
// MTrack
void MTrack_init(MTrack *mt, uint32_t size) {
  mt->events = (TrackEvent **)malloc(sizeof(TrackEvent *) * size);
  mt->len = 0;
  mt->size = size;
}
void MTrack_resize(MTrack *mt, uint32_t size) {
  assert(size > 0);
  // shrink
  if (mt->len > size) {
    for (int i = size; i < mt->len; i++) {
      TrackEvent_release(mt->events[i]);
    }
  }
  mt->events = (TrackEvent **)realloc(mt->events, sizeof(TrackEvent *) * size);
  mt->size = size;
}
void MTrack_append(MTrack *mt, TrackEvent *ev) {
  assert(mt->size < mt->len);
  if (mt->size == mt->len) {
    MTrack_resize(mt, mt->size * 2);
  }
  mt->events[mt->len] = ev;
  mt->len += 1;
}
// TODO MTrack_remove
void MTrack_release(MTrack *mt) {
  free(mt->events);
}

// MIDI event
void MidiEvent_init(TrackEvent *ev, uint32_t delta, MidiMsgType msg, uint8_t note, uint8_t vel, uint8_t ch) {
  ev->type = TrackEventType_MIDI;
  ev->delta = delta;
  ev->midi.msg = msg;
  ev->midi.ch = ch;
  ev->midi.note = note;
  ev->midi.vel = vel;
}

// Note ON
inline void
MidiEvent_set_note_on(TrackEvent *ev, uint32_t delta, uint8_t note, uint8_t vel, uint8_t ch) {
  MidiEvent_init(ev, delta, MidiMsgType_NOTE_ON, note, vel, ch);
}

// Note OFF
inline void
MidiEvent_set_note_off(TrackEvent *ev, uint32_t delta, uint8_t note, uint8_t ch) {
  MidiEvent_init(ev, delta, MidiMsgType_NOTE_OFF, note, 0, ch);
}

// SysEx Event
void SysExEvent_init(TrackEvent *ev, uint32_t delta, SysExType type, size_t len) {
  ev->type = TrackEventType_SYSEX;
  ev->delta = delta;
  ev->sysex.type = type;
  ev->sysex.len = len;
  ev->sysex.data = (uint8_t *)malloc(len);
}
void SysExEvent_release(TrackEvent *ev) {
  free(ev->sysex.data);
}

// Meta event
void MetaEvent_init(TrackEvent *ev, uint32_t delta, MetaEventType type, size_t len) {
  ev->type = TrackEventType_META;
  ev->delta = delta;
  ev->meta.type = type;
  ev->meta.len = len;
  if (len > 0)
    ev->meta.data = (uint8_t *)malloc(len);
  else
    ev->meta.data = NULL;
}
void MetaEvent_release(TrackEvent *ev) {
  free(ev->meta.data);
}
void MetaEvent_init_endoftrack(TrackEvent *ev, uint32_t delta) {
  MetaEvent_init(ev, delta, MetaEventType_SetTempo, 0);
}
void MetaEvent_init_tempo(TrackEvent *ev, uint32_t delta, uint32_t tempo) {
  MetaEvent_init(ev, delta, MetaEventType_SetTempo, 3);
  for (int i=0; i<3; i++) {
    ev->meta.data[i] = tempo >> (8 *(2 - i)) & 0xFF;
  }
}
void MetaEvent_init_time_signature(TrackEvent *ev, uint32_t delta, TimeSignature *ts) {
  MetaEvent_init(ev, delta, MetaEventType_TimeSignature, 4);
  ev->meta.data[0] = ts->numer;
  ev->meta.data[1] = ts->denom / 2;
  ev->meta.data[2] = ts->num_clocks;
  ev->meta.data[3] = ts->num_32nd;
}

// TrackEvent
void TrackEvent_release(TrackEvent *ev) {
  if (ev->type == TrackEventType_SYSEX)
    SysExEvent_release(ev);
  else if (ev->type == TrackEventType_META)
    MetaEvent_release(ev);
}
uint32_t TrackEvent_length(TrackEvent *ev) {
  VLUint vlu;
  VLUint_set(&vlu, ev->delta);
  if (ev->type == TrackEventType_MIDI) {
    if (ev->midi.msg == MidiMsgType_PROGRAM_CHANGE) {
      return vlu.size + 2;
    } else if (ev->midi.msg == MidiMsgType_CHANNEL_PRESSURE) {
      return vlu.size + 2;
    } else {
      return vlu.size + 3;
    }
  } else if (ev->type == TrackEventType_SYSEX) {
    if (ev->sysex.type == SysExType_F0) {
      return vlu.size + 3 +ev->sysex.len;
    } else {
      return vlu.size + 2 +ev->sysex.len;
    }
  } else if (ev->type == TrackEventType_META) {
    return vlu.size + 3 +ev->meta.len;
  }
  return vlu.size;
}