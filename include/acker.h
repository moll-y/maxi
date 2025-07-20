#ifndef ACKER_H_
#define ACKER_H_

#include <stdint.h>

struct acker {
  uint32_t bitfield;
  uint8_t local_sequence;
  uint8_t remote_sequence;
  uint8_t received_remote_sequences[32];
  uint8_t index;
};

int acker_make(struct acker *acker);

void acker_wrap(struct acker *acker, uint8_t *packet);

void acker_unwrap(struct acker *acker, uint8_t *packet);

#endif
