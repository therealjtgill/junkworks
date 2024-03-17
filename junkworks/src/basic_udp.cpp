#include "junkworks/basic_udp.hpp"

#include <iostream>

UdpSocket::UdpSocket(const unsigned int bind_port)
   : socket_handle_(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
   , bound_(bind_to(bind_port))
   , bind_port_(bound_ ? bind_port : 0)
{ }

UdpSocket::~UdpSocket(void)
{
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
   close(socket_handle_);
#elif PLATFORM == PLATFORM_WINDOWS
   closesocket(socket_handle_);
#endif
}

bool UdpSocket::bind_to(const unsigned int bind_port)
{
   if (socket_handle_ < 0)
   {
      return false;
   }

   // htons = host to network short. used to convert host byte order to
   // network byte order. required whenever you set integer members in socket
   // structures.
   // htonl = host to network long.

   unsigned int temp_port = bind_port;
   const uint16_t short_port = static_cast<unsigned short>(temp_port);

   sockaddr_in address;
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons(short_port);

   if (
      bind(
         socket_handle_,
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
         socket_handle_, F_SETFL, O_NONBLOCK, non_blocking
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

// void UdpSocket::close(void)
// {
// #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
//    close(socket_handle_);
// #elif PLATFORM == PLATFORM_WINDOWS
//    closesocket(socket_handle_);
// #endif
// }

bool UdpSocket::try_send(
   const Ipv4Address dest_ip,
   const unsigned int dest_port,
   const char * data,
   const unsigned int data_len
) const
{
   sockaddr_in send_address;
   send_address.sin_family = AF_INET;
   send_address.sin_addr.s_addr = dest_ip.internet_address();
   send_address.sin_port = htons(dest_port);

   const int num_bytes_sent = sendto(
      socket_handle_,
      data,
      data_len,
      0,
      (sockaddr*)&send_address,
      sizeof(sockaddr_in)
   );

   const bool length_match = (static_cast<const unsigned int>(num_bytes_sent) != data_len);

   if (length_match)
   {
      std::cout << "Failed to send packet, num bytes sent: " << num_bytes_sent << " num expected: " << data_len << "\n";
   }

   return length_match;
}

int UdpSocket::try_receive(
   const unsigned int max_data_len,
   char * data
) const
{
   sockaddr_in from_address;
   // any packet larger than 'max_packet_size' will be dropped
   unsigned int from_address_size = sizeof(from_address);
   int num_bytes_received = recvfrom(
      socket_handle_,
      data,
      max_data_len,
      0,
      (sockaddr*)&from_address,
      &from_address_size
   );

   return num_bytes_received;
}

bool UdpSocket::bound(void) const
{
   return bound_;
}

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

   unsigned int temp_port = port;
   const uint16_t short_port = static_cast<unsigned short>(temp_port);

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

void close_socket(const int handle)
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
