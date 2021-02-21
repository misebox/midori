#pragma once

#include <stdint.h>

// Midi Event
typedef enum _MidiMsgType {
  MidiMsgType_NOTE_OFF                = 0x80,
  MidiMsgType_NOTE_ON                 = 0x90,
  MidiMsgType_POLYPHONIC_KEY_PRESSURE = 0xA0,
  MidiMsgType_CONTROL_CHANGE          = 0xB0,
  MidiMsgType_PROGRAM_CHANGE          = 0xC0,
  MidiMsgType_CHANNEL_PRESSURE        = 0xD0,
  MidiMsgType_PITCH_BEND              = 0xE0,
} MidiMsgType;
typedef struct _MidiEvent {
  uint32_t len;
  MidiMsgType msg;
  uint8_t ch;
  uint8_t vel;
  uint8_t note;
} MidiEvent;

// SysEx Event
typedef enum _SysExType {
  // The format that starts with F0 and end with F7
  SysExType_F0 = 0xF0,
  // The format that starts with F7 and end with nothing
  SysExType_F7 = 0xF7,
} SysExType;
typedef struct _SysExEvent {
  SysExType type;
  uint32_t len;
  uint8_t *data;
} SysExEvent;

// Meta Event
typedef enum _MetaEventType {
  MetaEventType_SequenceNumber    = 0x00,
  MetaEventType_TextEvent         = 0x01,
  MetaEventType_CopyrightNotice   = 0x02,
  MetaEventType_SequenceTrackName = 0x03,
  MetaEventType_InstrumentName    = 0x04,
  MetaEventType_Lyric             = 0x05,
  MetaEventType_Marker            = 0x06,
  MetaEventType_CuePoint          = 0x07,
  MetaEventType_MIDIChannelPrefix = 0x20,
  MetaEventType_EndOfTrack        = 0x2F,
  MetaEventType_SetTempo          = 0x51,
  MetaEventType_SMPTEOffset       = 0x54,
  MetaEventType_TimeSignature     = 0x58,
  MetaEventType_KeySignature      = 0x59,
  MetaEventType_SequencerSpecific = 0x7F,
} MetaEventType;
typedef struct _METAEvent {
  MetaEventType type;
  uint32_t len;
  uint8_t *data;
} METAEvent;

// TrackEvent
typedef enum _TrackEventType {
  TrackEventType_MIDI,
  TrackEventType_SYSEX,
  TrackEventType_META,
} TrackEventType;
typedef struct _TrackEvent{
  TrackEventType type;
  uint32_t delta;
  union {
    MidiEvent midi;
    SysExEvent sysex;
    METAEvent meta;
  };
} TrackEvent;

// Variable Length Unsigned Integer
typedef struct _VLUint {
  uint8_t size;
  // If 2 or more bytes value, MSB of first byte is 1 and MSB of other byte is 0.
  // If value is lesser then 0x80, MSB of first byte is 0.
  // max 28bits value is representated
  uint8_t bytes[4];
} VLUint;

typedef struct _TimeSignature {
  uint8_t numer;
  uint8_t denom;
  uint8_t num_clocks;
  uint8_t num_32nd;
} TimeSignature;

// Header Chunk
typedef struct _MHead {
  uint16_t format;
  uint16_t trackcount;
  uint16_t resolution;
} MHead;

// Truck Chunk
typedef struct _MTrack {
  uint32_t len;
  uint32_t size;
  TrackEvent **events;
} MTrack;

typedef struct _SMF {
  MHead head;
  // There is no supports for format1 yet, so it's only one track.
  MTrack track;
  uint32_t tempo;
  TimeSignature ts;
} SMF;

// SMF
void VLUint_set(VLUint *vlu, uint32_t value);
void TimeSignature_init(TimeSignature *ts);

void MidiEvent_init(TrackEvent *ev, uint32_t delta, MidiMsgType msg, uint8_t note, uint8_t vel, uint8_t ch);
inline void MidiEvent_set_note_on(TrackEvent *ev, uint32_t delta, uint8_t note, uint8_t vel, uint8_t ch);
inline void MidiEvent_set_note_off(TrackEvent *ev, uint32_t delta, uint8_t note, uint8_t ch);
void SysExEvent_init(TrackEvent *ev, uint32_t delta, SysExType type, size_t len);
void MetaEvent_init(TrackEvent *ev, uint32_t delta, MetaEventType type, size_t len);
void MetaEvent_init_tempo(TrackEvent *ev, uint32_t delta, uint32_t tempo);
void MetaEvent_init_time_signature(TrackEvent *ev, uint32_t delta, TimeSignature *ts);
void MetaEvent_init_endoftrack(TrackEvent *ev, uint32_t delta);
void TrackEvent_release(TrackEvent *ev);
uint32_t TrackEvent_length(TrackEvent *ev);

// MHead
void MHead_init(MHead *head);
// MTrack
void MTrack_init(MTrack *mt, uint32_t size);
void MTrack_resize(MTrack *mt, uint32_t size);
void MTrack_append(MTrack *mt, TrackEvent *ev);
void MTrack_release(MTrack *mt);

void SMF_init(SMF *smf);
void SMF_release(SMF *smf);
