#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <person.h>
#include <stdint.h>
#include "midi.h"

int main() {
  FILE *fp = NULL;
  PERSON p;

  memset(p.name, 0xff, sizeof(p.name));
  snprintf(p.name, sizeof(p.name), "Hanako Yamda");
  p.age = 20;

  if ((fp = fopen("foo.dat", "wb")) == NULL) {
    perror(NULL);
    return 1;
  }

  fwrite(&p, sizeof(PERSON), 1, fp);

  fclose(fp);
  return 0;
}

void exit_failure(char *message) {
  fputs(message, stderr);
  exit(EXIT_FAILURE);
}

void write_or_exit(FILE *fp, const void *data, uint8_t size, size_t num) {
  if (fwrite(data, size, num, fp) < 1) {
    exit_failure("Failed to write VLUint to file");
  }
}

void write_vluint(FILE *fp, uint32_t value) {
  VLUint vlu;
  set_vluint(&vlu, value);
  if (vlu.size == 1) {
    write_or_exit(fp, vlu.bytes, 1, 1);
  } else {
    write_or_exit(fp, &vlu.size, 1, 1);
    write_or_exit(fp, &vlu.bytes, 1, vlu.size);
  }
}

void write_head(FILE *fp, MHead *head) {
  uint32_t length = 6;
  write_or_exit(fp, "MThd", 1, 4);
  write_or_exit(fp, &length, 4, 1);
  write_or_exit(fp, &head->format, 2, 1);
  write_or_exit(fp, &head->trackcount, 2, 1);
  write_or_exit(fp, &head->resolution, 2, 1);
}

void write_tempo(FILE *fp, uint32_t tempo) {
  write_or_exit(fp, MetaStart, 1, 1);
  write_or_exit(fp, 0x51, 1, 1);
  write_vluint(fp, tempo);
}

void write_time_signature(FILE *fp, TimeSignature *ts) {
  write_or_exit(fp, MetaStart, 1, 1);
  write_or_exit(fp, 0x58, 1, 1);
  write_or_exit(fp, ts->numer, 1, 1);
  write_or_exit(fp, ts->denom/2, 1, 1);
  write_or_exit(fp, ts->num_clocks, 1, 1);
  write_or_exit(fp, ts->num_32nd, 1, 1);
}

void write_smf(const char *filename) {
  uint32_t tempo = 120;
  MHead head = {0, 1, 480};
  TrackEvent *events;
  MTrack track = {};
  TimeSignature ts = {4, 4, 24, 8};

  FILE* fp = fopen(filename, "wb");
  if (fp == NULL)
    exit_failure("Failed to write to file\n");

  write_head(fp, &head);
  write_time_signature(fp, &ts);
  write_tempo(fp, &tempo);

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