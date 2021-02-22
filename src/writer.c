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
    bytes[i] = value >> ((3 - i) * 8) & 0xFF;
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
void Writer_or_exit(Writer *w, const void const *data, size_t size, size_t num) {
  cnt += size * num;
  debug_printf("cnt: %d", cnt);
  if (fwrite(data, size, num, w->fp) < 1) {
    exit_failure("Failed to write VLUint to file");
  }
}

void Writer_head(Writer *w, MHead *head) {
  Writer_or_exit(w, "MThd", 1, 4);
  uint32_t len = 6;
  uint8_t bytes[4];
  bytes_from_uint32(bytes, len);
  Writer_or_exit(w, bytes, 1, 4);
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
void Writer_meta_event(Writer *w, METAEvent *meta) {
  debug_printf("meta type %d", meta->type);
  Writer_or_exit(w, &meta->type, 1, 1);
  debug_printf("meta len %d", meta->len);
  Writer_or_exit(w, &meta->len, 1, 1);
  if (meta->len > 0) {
    debug_printf("meta %d", meta->len);
    Writer_or_exit(w, meta->data, 1, meta->len);
  }
}
void Writer_vluint(Writer *w, uint32_t value) {
  VLUint vlu;
  VLUint_set(&vlu, value);
  debug_printf("VLU: %d", value);
  if (vlu.size == 1) {
    Writer_or_exit(w, vlu.bytes, 1, 1);
  } else {
    Writer_or_exit(w, &vlu.size, 1, 1);
    Writer_or_exit(w, &vlu.bytes, vlu.size, 1);
  }
  debug_print("ok");
}
void Writer_trackevent(Writer *w, TrackEvent *ev) {
  Writer_vluint(w, ev->delta);
  if (ev->type == TrackEventType_MIDI) {
    Writer_midi_event(w, &ev->midi);
  } else if (ev->type == TrackEventType_SYSEX) {
    Writer_sysex_event(w, &ev->sysex);
  } else if (ev->type == TrackEventType_META) {
    Writer_meta_event(w, &ev->meta);
  }
  debug_print("ok");
}
void Writer_track(Writer *w, MTrack* track) {
  debug_printf("%d", track->len);
  Writer_or_exit(w, "MTrk", 1, 4);
  uint32_t len = 0;
  for (int i=0; i<track->len; i++) {
    len += TrackEvent_length(track->events[i]);
  }
  debug_printf("total len: %d", len);
  uint8_t bytes[4];
  bytes_from_uint32(bytes, len);
  Writer_or_exit(w, bytes, 1, 4);
  for (int i=0; i<track->len; i++) {
    debug_printf("write trackevent: %d", i);
    Writer_trackevent(w, track->events[i]);
  }
}

void Writer_smf(Writer *w, SMF *smf) {
  Writer_head(w, &smf->head);
  Writer_track(w, &smf->track);
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