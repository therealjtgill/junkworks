#ifndef BASIC_UDP_HEADER
#define BASIC_UDP_HEADER

#ifdef __cplusplus
extern "C"
{
#endif

#include "junkworks/network_common.h"

int initialize_sockets(void);

void shutdown_sockets(void);

int create_socket(void);

int bind_socket(const int handle, const unsigned int port);

void close_socket(const int handle);

int send_packet(
   const int handle,
   const sockaddr_in to_address,
   const char * data,
   const unsigned int packet_size
);

int receive_packet(
   const int socket,
   const unsigned int max_packet_size,
   sockaddr_in * from_address,
   char * packet_data
);

#ifdef __cplusplus
}
#endif

#endif
