#include <stdint.h>
#include <assert.h>

#include "midi.h"

void set_vluint(VLUint *vlu, uint32_t value) {
  assert(value < 0x10000000);
  int cur = 0;
  for (int i=0; i<=3; i++) {
    uint8_t byte = value >> (7 * (3 - i)) & 0xFF;
    if (cur > 0 || byte > 0 || i == 3) {
      vlu->bytes[cur] = byte;
      cur++;
    }
  }
  if (cur > 1) {
    vlu->bytes[0] |= 0x80;
  }
  vlu->size = cur;
}

void set_midi_event(MIDIEvent *ev, uint8_t note, uint8_t vel) {

}