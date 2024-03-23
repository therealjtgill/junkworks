#include "junkworks/basic_udp.h"

#include <stdint.h>
#include <stdio.h>

int initialize_sockets(void)
{
#if PLATFORM == PLATFORM_WINDOWS
   WSADATA wsa_data;
   return WSAStartup( MAKEWORD(2,2), &wsa_data) == NO_ERROR;
#else
   return 1;
#endif
}

void shutdown_sockets(void)
{
#if PLATFORM == PLATFORM_WINDOWS
   WSACleanup();
#endif
}

int create_socket(void)
{
   // create the socket
   int handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (handle <= 0)
   {
      printf("Failed to create socket\n");
   }

   return handle;
}

int bind_socket(const int handle, const unsigned int port)
{
   // htons = host to network short. used to convert host byte order to
   // network byte order. required whenever you set integer members in socket
   // structures.
   // htonl = host to network long.

   unsigned int temp_port = port;
   const uint16_t short_port = (unsigned short )temp_port;

   sockaddr_in address;
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons(short_port);

   if (
      bind(
         handle,
         (const sockaddr*)&address,
         sizeof(sockaddr_in)
      ) < 0
   )
   {
      printf("Failed to bind socket\n");
      return 0;
   }

   // set socket as non-blocking
#if PLATFORM == PLATFORM_MAC || \
    PLATFORM == PLATFORM_UNIX
   int non_blocking = 1;
   if (
      fcntl(
         handle, F_SETFL, O_NONBLOCK, non_blocking
      ) == 1
   )
   {
      printf("Failed to set port as non-blocking\n");
      return 0;
   }
#elif PLATFORM == PLATFORM_WINDOWS
   DWORD non_blocking = 1;
   if (
      ioctlsocket(
         handle, FIONBIO, &non_blocking
      ) != 0
   )
   {
      printf("Failed to set port as non-blocking\n");
      return 0;
   }
#endif

   return 1;
}

void close_socket(const int handle)
{
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
   close(handle);
#elif PLATFORM == PLATFORM_WINDOWS
   closesocket(handle);
#endif
}

int send_packet(
   const int handle,
   const sockaddr_in to_address,
   const char * data,
   const unsigned int packet_size
)
{
   const unsigned int sent_bytes = sendto(
      handle,
      data,
      packet_size,
      0,
      (sockaddr*)&to_address,
      sizeof(sockaddr_in)
   );

   if (sent_bytes != packet_size)
   {
      printf("Failed to send packet\n");
   }

   return (sent_bytes != packet_size);
}

int receive_packet(
   const int socket,
   const unsigned int max_packet_size,
   sockaddr_in * from_address,
   char * packet_data
)
{
   // any packet larger than 'max_packet_size' will be dropped
   unsigned int from_address_size = sizeof(from_address);
   int bytes = recvfrom(
      socket,
      packet_data,
      max_packet_size,
      0,
      (sockaddr*)from_address,
      &from_address_size
   );

   return bytes > 0;
}
