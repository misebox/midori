#pragma once

#include <stdio.h>
#include "midi.h"

typedef struct _Writer {
  FILE *fp;
  const char *filename;
} Writer;

void Writer_write_init(Writer *w, const char *filename);
void Writer_write_release(Writer *w);

void exit_failure(char *message);
void Writer_write_open(Writer *w, const char *filename);
void Writer_write_close(Writer *w);
void Writer_write_or_exit(Writer *w, const void const *data, size_t size, size_t num);
void Writer_write_uint16_or_exit(Writer *w, uint16_t value);
void Writer_write_uint32_or_exit(Writer *w, uint32_t value);
// head
void Writer_write_head(Writer *w, MHead *head);
// track_events
void Writer_write_midi_event(Writer *w, MidiEvent *midi);
void Writer_write_sysex_event(Writer *w, SysExEvent *sysex);
void Writer_write_meta_event(Writer *w, METAEvent *meta);
void Writer_write_vluint(Writer *w, uint32_t value);
void Writer_write_trackevent(Writer *w, TrackEvent *ev);
void Writer_write_track(Writer *w, MTrack* track);

void Writer_write_smf(Writer *w, SMF *smf);
