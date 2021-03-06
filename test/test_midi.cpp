#include <gtest/gtest.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "debug.h"
#include "midi.h"


void expect_vlu(VLUint *vlu, uint8_t size, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
  EXPECT_EQ(vlu->size, size);
  EXPECT_EQ(vlu->bytes[0], b1);
  EXPECT_EQ(vlu->bytes[1], b2);
  EXPECT_EQ(vlu->bytes[2], b3);
  EXPECT_EQ(vlu->bytes[3], b4);
}

TEST(VLUint_set, Set1Byte) {
  VLUint vlu;
  VLUint_set(&vlu, 0);
  expect_vlu(&vlu, 1, 0, 0, 0, 0);
  VLUint_set(&vlu, 0x78);
  expect_vlu(&vlu, 1, 0x78, 0, 0, 0);
  VLUint_set(&vlu, 0x7f);
  expect_vlu(&vlu, 1, 0x7f, 0, 0, 0);
}
TEST(VLUint_set, Set2Byte) {
  VLUint vlu;
  VLUint_set(&vlu, 0x80);
  expect_vlu(&vlu, 2, 0x81, 0, 0, 0);
  VLUint_set(&vlu, 0x100);
  expect_vlu(&vlu, 2, 0x82, 0, 0, 0);
  VLUint_set(&vlu, 0x3fff);
  expect_vlu(&vlu, 2, 0xff, 0x7f, 0, 0);
}
// void VLUint_set(VLUint *vlu, uint32_t value);
// void TimeSignature_init(TimeSignature *ts);
// 
// void MidiEvent_init(TrackEvent *ev, uint32_t delta, MidiMsgType msg, uint8_t note, uint8_t vel, uint8_t ch);
// void MidiEvent_set_note_on(TrackEvent *ev, uint32_t delta, uint8_t note, uint8_t vel, uint8_t ch);
// void MidiEvent_set_note_off(TrackEvent *ev, uint32_t delta, uint8_t note, uint8_t ch);
// void SysExEvent_init(TrackEvent *ev, uint32_t delta, SysExType type, size_t len);
// void MetaEvent_init(TrackEvent *ev, uint32_t delta, MetaEventType type, size_t len);
// void MetaEvent_init_tempo(TrackEvent *ev, uint32_t delta, uint32_t tempo);
// void MetaEvent_init_time_signature(TrackEvent *ev, uint32_t delta, TimeSignature *ts);
// void MetaEvent_init_endoftrack(TrackEvent *ev, uint32_t delta);
// void TrackEvent_release(TrackEvent *ev);
// uint32_t TrackEvent_length(TrackEvent *ev);
// 
// // MHead
// void MHead_init(MHead *head);
// // MTrack
// void MTrack_init(MTrack *mt, uint32_t size);
// void MTrack_resize(MTrack *mt, uint32_t size);
// void MTrack_append(MTrack *mt, TrackEvent *ev);
// void MTrack_release(MTrack *mt);
// 
// void SMF_init(SMF *smf);
// void SMF_release(SMF *smf);