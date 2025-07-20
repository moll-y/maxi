#include "acker.h"
#include "packet.h"
#include "udp.h"
#include <stdio.h>
#include <unistd.h>

#define PACKET_RATE_HZ 30
#define INTERVAL_US (1000000 / PACKET_RATE_HZ)

int main(int argc, char *argv[]) {
  struct acker acker;
  acker_make(&acker);

  struct udp udp;
  if (udp_make(&udp, &acker) < 0) {
    return -1;
  }

  while (1) {
    usleep(INTERVAL_US);
    uint8_t packet[PACKET_LENGTH];

    const ssize_t sent_bytes = udp_send(&udp, packet, sizeof(packet));
    if (sent_bytes < 0) {
      fprintf(stderr, "Could not send packet.\n");
    }

    const ssize_t received_bytes = udp_recv(&udp, packet, sizeof(packet));
    if (received_bytes < 0) {
      fprintf(stderr, "Could not receive packet.\n");
    }
  }

  udp_free(&udp);
  return 0;
}
