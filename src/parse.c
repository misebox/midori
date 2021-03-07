#include "parse.h"
#include "token.h"
#include "vec.h"
#include "midi.h"
#include "debug.h"
#include "util.h"


Token *next_token(Vec *v, size_t i) {
  return i < v->len ? v->items[i] : NULL;
}
void parse(SMF *smf, Vec *tokens) {
  // SMF Data
  TrackEvent *tempo_ev = MetaEvent_new_tempo((uint32_t)0, smf->tempo);
  debug_printf("delta: %d", tempo_ev->delta);
  MTrack_append(&smf->track, tempo_ev);
  TrackEvent *ts_ev = MetaEvent_new_time_signature((uint32_t)0, &smf->ts);
  MTrack_append(&smf->track, ts_ev);

  Vec *values = Vec_new(10);
  Vec *operators = Vec_new(10);
  size_t cur = 0;
  while (cur < tokens->len) {
    Token *t = next_token(tokens, cur++);
    //if (Token_is_value_token(t)) {
    //  Token_push(values, t);
    //} else if (Token_is_operator_token(t)) {
    //  Token_push(operators, t);
    //}
    if (t->ty == TokenType_PHRASE) {
      t = next_token(tokens, cur++);
      if (t->ty != TokenType_QUOTED) {
        exit_failure("QUOTED not found after PHRASE");
      }
    }
  }
  for (size_t i=0; i<tokens->len; i++) {
    Token_print(tokens->items[i]);
  }
  // TrackEvent *melody = (TrackEvent *)malloc(sizeof(TrackEvent) * 100);
  // for (int i=0; i<61; i++) {
  //   TrackEvent *ev;
  //   ev = TrackEvent_new();
  //   MidiEvent_init(ev, 0, MidiMsgType_NOTE_ON, 0x30 + i, 100, 0);
  //   MTrack_append(&smf->track, ev);
  //   ev = TrackEvent_new();
  //   MidiEvent_init(ev, 120, MidiMsgType_NOTE_OFF, 0x30 + i, 0, 0);
  //   MTrack_append(&smf->track, ev);
  // }

  TrackEvent *end_ev = MetaEvent_new_endoftrack(0);
  debug_printf("end_ev.len: %d", end_ev->meta.len);
  MTrack_append(&smf->track, end_ev);
}