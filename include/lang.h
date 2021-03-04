#pragma once

#include "vec.h"
#include "token.h"
/*
typedef enum EUnitType {
  UnitType_Meta,
  UnitType_Note,
  UnitType_Rest,
  UnitType_Repeat,
  UnitType_Group,
  UnitType_Phrase,
} UnitType;

typedef enum EUnitMetaType {
  Duration,
  Octave,
} UnitMetaType;
typedef struct _Meta {
} Meta;

typedef struct _Note {
  int oct;
  int no; // 0-12
  size_t dur;
} Note;

typedef struct _Rest {
  size_t dur;
} Rest;

typedef struct _Repeat {
  size_t count;
} Repeat;

typedef struct _Phrase {
  size_t size;
  size_t len;
  int baseoct;
  size_t basedur;
  Unit **list;
} Phrase;
typedef struct Unit {
  UnitType ty;
  union {
    Note note;
    Rest rest;
    Repeat repeat;
    Repeat repeat;
  }
};

typedef enum EPhraseType { PhraseType_Note, PhraseType_List } PhraseType;

*/