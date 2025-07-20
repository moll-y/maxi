#include "acker.h"
#include <stdint.h>

int acker_make(struct acker *acker) {
  acker->local_sequence = 0;
  acker->remote_sequence = 0;
  acker->bitfield = 0;
  acker->index = 0;
  return 0;
}

void acker_wrap(struct acker *acker, uint8_t *packet) {
  const int size = 32;
  acker->bitfield = 0;
  for (int i = 1; i < size; i++) {
    const uint8_t s = (acker->remote_sequence - i) & 0xFFFF;
    for (int j = 0; j < size; j++) {
      if (s == acker->received_remote_sequences[j]) {
        acker->bitfield |= (1 << (i - 1));
        break;
      }
    }
  }
  packet[1] = acker->local_sequence;
  packet[2] = acker->remote_sequence;
  packet[3] = acker->bitfield;
  acker->local_sequence++;
}

void acker_unwrap(struct acker *acker, uint8_t *packet) {
  acker->remote_sequence = packet[1];
  acker->received_remote_sequences[acker->index] = acker->remote_sequence;
  acker->index = (acker->index + 1) % 32;
}
