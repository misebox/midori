#include <stdio.h>
#include <stdlib.h>
#include "writer.h"
#include "midi.h"


void exit_failure(char *message) {
  fputs(message, stderr);
  exit(EXIT_FAILURE);
}

void Writer_open(Writer *w, const char *filename) {
  if ((w->fp = fopen(filename, "wb")) == NULL) {
    perror(NULL);
    exit(EXIT_FAILURE);
  }
}
void Writer_close(Writer *w) {
  // End check?
  fclose(w->fp);
}
void Writer_or_exit(Writer *w, const void const *data, size_t size, size_t num) {
  if (fwrite(data, size, num, w->fp) < 1) {
    exit_failure("Failed to write VLUint to file");
  }
}

void Writer_head(Writer *w, MHead *head) {
  Writer_or_exit(w, "MThd", 1, 4);
  uint32_t length = 6;
  Writer_or_exit(w, &length, 4, 1);
  Writer_or_exit(w, &head->format, 2, 1);
  Writer_or_exit(w, &head->trackcount, 2, 1);
  Writer_or_exit(w, &head->resolution, 2, 1);
}

// Events
void Writer_midi_event(Writer *w, MidiEvent *midi) {
  uint8_t status = midi->msg | (midi->ch & 0x0F);
  Writer_or_exit(w, &status , 1, 1);
  Writer_or_exit(w, &midi->note, 1, 1);
  Writer_or_exit(w, &midi->vel, 1, 1);
}
void Writer_sysex_event(Writer *w, SysExEvent *sysex) {
  Writer_or_exit(w, &sysex->type, 1, 1);
  Writer_or_exit(w, &sysex->len, 1, 1);
  if (sysex->len > 0) {
    Writer_or_exit(w, sysex->data, 1, sysex->len);
  }
  if (sysex->type == SysExType_F0) {
    uint8_t endbyte = 0xF7;
    Writer_or_exit(w, &endbyte, 1, 1);
  }
}
const uint8_t MetaStart = 0xFF;
void Writer_meta_event(Writer *w, METAEvent *meta) {
  Writer_or_exit(w, &MetaStart, 1, 1);
  Writer_or_exit(w, &meta->type, 1, 1);
  Writer_or_exit(w, &meta->len, 1, 1);
  if (meta->len > 0) {
    Writer_or_exit(w, meta->data, 1, meta->len);
  }
}
void Writer_vluint(Writer *w, uint32_t value) {
  VLUint vlu;
  VLUint_set(&vlu, value);
  if (vlu.size == 1) {
    Writer_or_exit(w, vlu.bytes, 1, 1);
  } else {
    Writer_or_exit(w, &vlu.size, 1, 1);
    Writer_or_exit(w, &vlu.bytes, 1, vlu.size);
  }
}
Writer_trackevent(Writer *w, TrackEvent *ev) {
  Writer_vluint(w, ev->delta);
  if (ev->type == TrackEventType_MIDI) {
    Writer_midi_event(w, ev);
  } else if (ev->type == TrackEventType_SYSEX) {
    Writer_sysex_event(w, ev);
  } else if (ev->type == TrackEventType_META) {
    Writer_meta_event(w, ev);
  }
}
void Writer_track(Writer *w, MTrack* track) {
  Writer_or_exit(w, "MTrk", 1, 4);
  uint32_t len = 0;
  for (int i=0; i<track->len; i++) {
    len += TrackEvent_length(track->events[i]);
  }
  Writer_or_exit(w, &len, 4, 1);
  for (int i=0; i<track->len; i++) {
    Writer_trackevent(w, &track->events[i]);
  }
}

void Writer_smf(const char *filename) {
  uint32_t tempo = 120;
  MHead head = {0, 1, 480};
  TrackEvent *events;
  MTrack track = {};
  TimeSignature ts = {4, 4, 24, 8};

  FILE* fp = fopen(filename, "wb");
  if (fp == NULL)
    exit_failure("Failed to write to file\n");

  Writer_head(fp, &head);
  Writer_time_signature(fp, &ts);
  Writer_tempo(fp, &tempo);

  if (fclose(fp) == EOF)
    exit_failure("Failed to close file\n");
}

/*
                4D 54 68 64     MThd
                00 00 00 06     ﾌﾞﾛｯｸ長
                00 00           ﾌｫｰﾏｯﾄ0
                00 01           1ﾄﾗｯｸ
                00 60           96/四分音符

次にトラック・ブロックである。ヘッダの後にイベントが続く（ランニング・
ステータスが随所で用いられていることに注意せよ）。

                4D 54 72 6B     MTrk
                00 00 00 3B     ﾌﾞﾛｯｸ長(59)

     ﾃﾞﾙﾀ･ﾀｲﾑ   イベント              　コメント
        00      FF 58 04 04 02 18 08    拍子記号
        00      FF 51 03 07 A1 20       テンポ
        00      C0 05
        00      C1 2E
        00      C2 46
        00      92 30 60
        00      3C 60                   ﾗﾝﾆﾝｸﾞ･ｽﾃｰﾀｽ
        60      91 43 40
        60      90 4C 20
        81 40   82 30 40                2ﾊﾞｲﾄのﾃﾞﾙﾀ･ﾀｲﾑ
        00      3C 40                   ﾗﾝﾆﾝｸﾞ･ｽﾃｰﾀｽ
        00      81 43 40
        00      80 4C 40
        00      FF 2F 00                end of track
*/