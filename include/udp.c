#include "udp.h"
#include "acker.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int udp_make(struct udp *udp, struct acker *acker) {
  const int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    fprintf(stderr, "Could not create AF_INET, SOCK_DGRAM socket.\n");
    return -1;
  }
  if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) {
    fprintf(stderr, "Could not set O_NONBLOCK for file descriptor.\n");
    return -1;
  }
  const struct sockaddr_in name = {
      .sin_family = AF_INET,
      .sin_port = htons(1053),
      .sin_addr.s_addr = htonl(INADDR_ANY),
  };
  socklen_t size = sizeof(name);
  udp->sockfd = sockfd;
  udp->acker = acker;
  udp->name = name;
  udp->size = size;
  return 0;
}

ssize_t udp_send(struct udp *udp, uint8_t *packet, uint8_t packet_size) {
  acker_wrap(udp->acker, packet);
  return sendto(udp->sockfd, packet, packet_size, 0,
                (struct sockaddr *)&udp->name, udp->size);
}

ssize_t udp_recv(struct udp *udp, uint8_t *packet, uint8_t packet_size) {
  acker_unwrap(udp->acker, packet);
  return recvfrom(udp->sockfd, packet, packet_size, 0,
                  (struct sockaddr *)&udp->name, &udp->size);
}

void udp_free(struct udp *udp) { close(udp->sockfd); }
