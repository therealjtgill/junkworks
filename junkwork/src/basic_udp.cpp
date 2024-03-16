#include "junkwork/basic_udp.hpp"

#include <iostream>

bool initialize_sockets(void)
{
#if PLATFORM == PLATFORM_WINDOWS
   WSADATA wsa_data;
   return WSAStartup( MAKEWORD(2,2), &wsa_data) == NO_ERROR;
#else
   return true;
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
      std::cout << "Failed to create socket\n";
   }

   return handle;
}

bool bind_socket(const int handle, const unsigned int port)
{
   // htons = host to network short. used to convert host byte order to
   // network byte order. required whenever you set integer members in socket
   // structures.
   // htonl = host to network long.
   sockaddr_in address;
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons( static_cast<const unsigned short>(port));

   if (
      bind(
         handle,
         (const sockaddr*)&address,
         sizeof(sockaddr_in)
      ) < 0
   )
   {
      std::cout << "Failed to bind socket\n";
      return false;
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
      std::cout << "Failed to set port as non-blocking\n";
      return false;
   }
#elif
   DWORD non_blocking = 1;
   if (
      isoctlsocket(
         handle, FIONBIO, &non_blocking
      ) != 0
   )
   {
      std::cout << "Failed to set port as non-blocking\n";
      return false;
   }
#endif

   return true;
}

void close_socket(
   const int handle
)
{
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
   close(handle);
#elif PLATFORM == PLATFORM_WINDOWS
   closesocket(handle);
#endif
}

bool send_packet(
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
      std::cout << "Failed to send packet\n";
   }

   return (sent_bytes != packet_size);
}

bool receive_packet(
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
