#include "junkworks/udp_socket.hpp"

#include "junkworks/basic_udp.h"
#include "junkworks/network_common.h"

#include <iostream>

namespace junkworks
{
   bool UdpSocket::sockets_initialized = false;

   bool UdpSocket::socket_initialization_successful = false;

   UdpSocket::UdpSocket(const unsigned int bind_port)
   {
      bound_ = false;

      if (!sockets_initialized)
      {
         socket_initialization_successful = initialize_sockets();
         sockets_initialized = true;
      }

      if (!socket_initialization_successful)
      {
         socket_handle_ = -1;
         bind_port_ = 0;
         return;
      }

      socket_handle_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      bound_ = bind_to(bind_port);
      bind_port_ = bound_ ? bind_port : 0;
   }

   UdpSocket::~UdpSocket(void)
   {
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
      close(socket_handle_);
#elif PLATFORM == PLATFORM_WINDOWS
      closesocket(socket_handle_);
#endif
   }

   bool UdpSocket::try_send(
      const ipv4add dest_ip,
      const unsigned int dest_port,
      const char * data,
      const unsigned int data_len
   ) const
   {
      sockaddr_in send_address;
      send_address.sin_family = AF_INET;
      send_address.sin_addr.s_addr = dest_ip.internet_address();
      send_address.sin_port = htons(dest_port);

      const unsigned int num_bytes_sent = sendto(
         socket_handle_,
         data,
         data_len,
         0,
         (sockaddr*)&send_address,
         sizeof(sockaddr_in)
      );

      const bool length_match = ((num_bytes_sent) != data_len);

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
      int from_address_size = 0;

      // any packet larger than 'max_packet_size' will be dropped
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

   void UdpSocket::receive_all(
      std::vector<raw_payload_t<128> > & payloads
   ) const
   {
      sockaddr_in from_address;
      int from_address_size = sizeof(from_address);

      raw_payload_t<128> temp_payload;
      temp_payload.size = 1;

      do
      {
         temp_payload.size = recvfrom(
            socket_handle_,
            temp_payload.data,
            128,
            0,
            (sockaddr*)&from_address,
            &from_address_size
         );

         temp_payload.ip = from_address.sin_addr.s_addr;

         if (temp_payload.size > 0)
         {
            payloads.push_back(temp_payload);
         }
      } while (temp_payload.size > 0);
   }

   bool UdpSocket::bound(void) const
   {
      return bound_;
   }

   bool UdpSocket::bind_to(const unsigned int bind_port)
   {
      if (socket_handle_ < 0)
      {
         std::cout << "bad socket handle: " << socket_handle_ << "\n";
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
#elif PLATFORM == PLATFORM_WINDOWS
      DWORD non_blocking = 1;
      if (
         ioctlsocket(
            socket_handle_, FIONBIO, &non_blocking
         ) != 0
      )
      {
         std::cout << "Failed to set port as non-blocking\n";
         return false;
      }
#endif

      return true;
   }

}
