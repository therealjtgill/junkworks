#ifndef BASIC_UDP_HEADER
#define BASIC_UDP_HEADER

#include "junkworks/network_common.h"

bool initialize_sockets(void);

void shutdown_sockets(void);

int create_socket(void);

bool bind_socket(const int handle, const unsigned int port);

void close_socket(const int handle);

bool send_packet(
   const int handle,
   const sockaddr_in to_address,
   const char * data,
   const unsigned int packet_size
);

bool receive_packet(
   const int socket,
   const unsigned int max_packet_size,
   sockaddr_in * from_address,
   char * packet_data
);

#endif
