#include <stdio.h>
#include <stdlib.h>
#include "writer.h"
#include "midi.h"
#include "debug.h"


void exit_failure(char *message) {
  fputs(message, stderr);
  exit(EXIT_FAILURE);
}

void bytes_from_uint32(uint8_t bytes[4], uint32_t value) {
  for (int i=0; i<4; i++) {
    bytes[i] = (value >> ((3 - i) * 8)) & 0xFF;
  }
}

int cnt;
void Writer_open(Writer *w, const char *filename) {
  cnt = 0;
  if ((w->fp = fopen(filename, "wb")) == NULL) {
    perror(NULL);
    exit_failure("Failed to write to file\n");
  }
}
void Writer_close(Writer *w) {
  cnt = 0;
  // End check?
  if (fclose(w->fp) == EOF)
    exit_failure("Failed to close file\n");
}
void Writer_write_or_exit(Writer *w, const void const *data, size_t size, size_t num) {
  cnt += size * num;
  debug_printf("cnt: %d", cnt);
  if (fwrite(data, size, num, w->fp) < 1) {
    exit_failure("Failed to write VLUint to file");
  }
}
void Writer_write_uint32_or_exit(Writer *w, uint32_t value) {
  debug_printf("value: %d", value);
  uint8_t bytes[4];
  bytes_from_uint32(bytes, value);
  Writer_write_or_exit(w, bytes, 1, 4);
}

void Writer_write_head(Writer *w, MHead *head) {
  Writer_write_or_exit(w, "MThd", 1, 4);
  Writer_write_uint32_or_exit(w, 6);

  Writer_write_or_exit(w, &head->format, 2, 1);
  Writer_write_or_exit(w, &head->trackcount, 2, 1);
  Writer_write_or_exit(w, &head->resolution, 2, 1);
}

// Events
void Writer_write_midi_event(Writer *w, MidiEvent *midi) {
  uint8_t status = midi->msg | (midi->ch & 0x0F);
  Writer_write_or_exit(w, &status, 1, 1);
  Writer_write_or_exit(w, &midi->note, 1, 1);
  Writer_write_or_exit(w, &midi->vel, 1, 1);
}
void Writer_write_sysex_event(Writer *w, SysExEvent *sysex) {
  Writer_write_or_exit(w, &sysex->type, 1, 1);
  Writer_write_or_exit(w, &sysex->len, 1, 1);
  if (sysex->len > 0) {
    Writer_write_or_exit(w, sysex->data, 1, sysex->len);
  }
  if (sysex->type == SysExType_F0) {
    uint8_t endbyte = 0xF7;
    Writer_write_or_exit(w, &endbyte, 1, 1);
  }
}
const uint8_t MetaStart = 0xFF;
void Writer_write_meta_event(Writer *w, METAEvent *meta) {
  debug_printf("meta type %d", meta->type);
  Writer_write_or_exit(w, &MetaStart, 1, 1);
  Writer_write_or_exit(w, &meta->type, 1, 1);
  debug_printf("meta len %d", meta->len);
  Writer_write_or_exit(w, &meta->len, 1, 1);
  if (meta->len > 0) {
    debug_printf("meta %d", meta->len);
    Writer_write_or_exit(w, meta->data, 1, meta->len);
  }
}
void Writer_write_vluint(Writer *w, uint32_t value) {
  VLUint vlu;
  VLUint_set(&vlu, value);
  debug_printf("VLU: %d", value);
  Writer_write_or_exit(w, vlu.bytes, 1, vlu.size);
  debug_print("vlu ok");
}
void Writer_write_trackevent(Writer *w, TrackEvent *ev) {
  if (ev->type == TrackEventType_META && ev->meta.type ==MetaEventType_EndOfTrack) {
    exit(EXIT_FAILURE);
  }
  Writer_write_vluint(w, ev->delta);
  if (ev->type == TrackEventType_MIDI) {
    Writer_write_midi_event(w, &ev->midi);
  } else if (ev->type == TrackEventType_SYSEX) {
    Writer_write_sysex_event(w, &ev->sysex);
  } else if (ev->type == TrackEventType_META) {
    Writer_write_meta_event(w, &ev->meta);
  }
  debug_print("ok");
}
void Writer_write_track(Writer *w, MTrack* track) {
  debug_printf("%d", track->len);
  Writer_write_or_exit(w, "MTrk", 1, 4);
  uint32_t len = 0;
  for (int i=0; i<track->len; i++) {
    len += TrackEvent_length(track->events[i]);
  }
  debug_printf("total len: %d", len);
  Writer_write_uint32_or_exit(w, len);
  for (int i=0; i<track->len; i++) {
    debug_printf("write trackevent: %d", i);
    Writer_write_trackevent(w, track->events[i]);
  }
}

void Writer_write_smf(Writer *w, SMF *smf) {
  Writer_write_head(w, &smf->head);
  Writer_write_track(w, &smf->track);
}
