#ifndef UDP_H_
#define UDP_H_

#include "acker.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct udp {
  int sockfd;
  struct sockaddr_in name;
  socklen_t size;
  struct acker *acker;
};

int udp_make(struct udp *udp, struct acker *acker);

ssize_t udp_send(struct udp *udp, uint8_t *packet, uint8_t packet_size);

ssize_t udp_recv(struct udp *udp, uint8_t *packet, uint8_t packet_size);

void udp_free(struct udp *udp);

#endif
