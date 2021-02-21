#pragma once

#include <stdint.h>

typedef struct _MHead {
  uint16_t format;
  uint16_t trackcount;
  uint16_t resolution;
} MHead;

typedef struct _MTrack {
  uint32_t len;
  TrackEvent *events;
} MTrack;

typedef enum _EventType {
  MIDI,
  SYSEX,
  META,
} EventType;

typedef enum _MidiMsg {
  NOTE_OFF                = 0x80,
  NOTE_ON                 = 0x90,
  POLYPHONIC_KEY_PRESSURE = 0xA0,
  CONTROL_CHANGE          = 0xB0,
  PROGRAM_CHANGE          = 0xC0,
  CHANNEL_PRESSURE        = 0xD0,
  PITCH_BEND              = 0xE0,
} MidiMsg;

typedef struct _MIDIEvent {
  uint32_t len;
  MidiMsg msg;
  uint8_t ch;
  uint8_t vel;
  uint8_t note;
} MIDIEvent;

typedef struct _SYSEXEvent {
  uint32_t len;
} SYSEXEvent;

typedef struct _METAEvent {
  uint32_t len;
  uint8_t *data;
} METAEvent;

typedef struct _TrackEvent{
  EventType type;
  uint32_t delta;
  union {
    MIDIEvent midi;
    SYSEXEvent sysex;
    METAEvent meta;
  };
} TrackEvent;

const uint8_t MetaStart = 0xFF;

typedef struct _VLUint {
  uint8_t size;
  // max32bit
  uint8_t bytes[4];
} VLUint;

typedef struct _TimeSignature {
  uint8_t numer;
  uint8_t denom;
  uint8_t num_clocks;
  uint8_t num_32nd;
} TimeSignature;
