#define PLATFORM_WINDOWS 1
#define PLATFORM_MAC     2
#define PLATFORM_UNIX    3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>

#elif PLATFORM == PLATFORM_MAC || \
      PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#endif

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
   unsigned int from_address_size = sizeof(from_address);
   int bytes = recvfrom(
      socket,
      packet_data,
      max_packet_size,
      0,
      (sockaddr*)from_address,
      &from_address_size
   );

   return bytes <= 0;
}

int main(void)
{
   int handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (handle <= 0)
   {
      std::cout << "Failed to create socket\n";
      return 1;
   }

   // htons = host to network short. used to convert host byte order to
   // network byte order. required whenever you set integer members in socket
   // structures.
   // htonl = host to network long.

   // create the socket
   unsigned int port = 8000;

   sockaddr_in address;
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons( static_cast<unsigned short>(port));

   if (
      bind(
         handle,
         // static_cast<const sockaddr*>(&address),
         (const sockaddr*)&address,
         sizeof(sockaddr_in)
      ) < 0
   )
   {
      std::cout << "Failed to bind socket\n";
      return 1;
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
      return 1;
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
      return 1;
   }
#endif

   unsigned int a = 127;
   unsigned int b = 0;
   unsigned int c = 0;
   unsigned int d = 1;

   unsigned int ip_add = (a << 24) | (b << 16) | ( c << 8) | d;

   address.sin_addr.s_addr = ip_add;

   return 0;
}
