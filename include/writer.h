#pragma once

#include <stdio.h>
#include "midi.h"

typedef struct _Writer {
  FILE *fp;
  const char *filename;
} Writer;

void Writer_init(Writer *w, const char *filename);
void Writer_release(Writer *w);

void exit_failure(char *message);
void Writer_open(Writer *w, const char *filename);
void Writer_close(Writer *w);
void Writer_or_exit(Writer *w, const void const *data, size_t size, size_t num);
// head
void Writer_head(Writer *w, MHead *head);
// track_events
void Writer_midi_event(Writer *w, MidiEvent *midi);
void Writer_sysex_event(Writer *w, SysExEvent *sysex);
void Writer_meta_event(Writer *w, METAEvent *meta);
void Writer_vluint(Writer *w, uint32_t value);
void Writer_trackevent(Writer *w, TrackEvent *ev);
void Writer_track(Writer *w, MTrack* track);

void Writer_smf(Writer *w, SMF *smf);
