#include "acker.h"
#include "packet.h"
#include <stdbool.h>
#include <stdint.h>

#define HALF_SEQUENCE_RANGE_8 128
#define BITFIELD_LENGTH 32
#define ARRAY_LENGTH 32

bool is_sequence_greater_than(uint8_t next, uint8_t actual);

void acker_make(struct acker *acker) {
  acker->local_sequence = 0;
  acker->remote_sequence = 0;
  acker->bitfield = 0;
  acker->index = 0;
  for (uint8_t i = 0; i < ARRAY_LENGTH; i++) {
    acker->received_remote_sequences[i] = 0;
  }
}

void acker_wrap(struct acker *acker, uint8_t *packet) {
  acker->bitfield = 0;
  for (uint8_t i = 1; i < BITFIELD_LENGTH; i++) {
    const uint8_t s = (acker->remote_sequence - i) & 0xFFFF;
    for (uint8_t j = 0; j < ARRAY_LENGTH; j++) {
      if (s == acker->received_remote_sequences[j]) {
        acker->bitfield |= (1 << (i - 1));
        break;
      }
    }
  }
  packet[PACKET_SEQUENCE] = acker->local_sequence;
  packet[PACKET_ACK] = acker->remote_sequence;
  packet[PACKET_ACK_BITFIELD] = acker->bitfield;
  acker->local_sequence++;
}

void acker_unwrap(struct acker *acker, uint8_t *packet) {
  if (!is_sequence_greater_than(packet[PACKET_SEQUENCE],
                                acker->remote_sequence)) {
    return;
  }
  acker->remote_sequence = packet[PACKET_SEQUENCE];
  acker->received_remote_sequences[acker->index] = packet[PACKET_SEQUENCE];
  acker->index = (acker->index + 1) % BITFIELD_LENGTH;
}

inline bool is_sequence_greater_than(uint8_t next, uint8_t actual) {
  return ((next > actual) && (next - actual <= HALF_SEQUENCE_RANGE_8)) ||
         ((next < actual) && (actual - next > HALF_SEQUENCE_RANGE_8));
}
