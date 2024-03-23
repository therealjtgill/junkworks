#ifndef UDP_SOCKET_HEADER
#define UDP_SOCKET_HEADER

#include "junkworks/ipv4add.hpp"
#include "junkworks/types.hpp"

#include <vector>

namespace junkworks
{
   class UdpSocket
   {
      public:
         static bool os_sockets_initialized;

         static bool os_socket_initialization_successful;

         UdpSocket(void) = delete;

         UdpSocket(const unsigned int bind_port);

         ~UdpSocket(void);

         bool try_send(
            const ipv4add dest_ip,
            const unsigned int dest_port,
            const char * data,
            const unsigned int data_len
         ) const;

         int try_receive(
            const unsigned int max_data_len,
            char * data
         ) const;

         void receive_all(std::vector<raw_payload_t<128> > & payloads) const;

         bool bound(void) const;

         unsigned int bind_port(void) const
         {
            return bind_port_;
         }

      private:
         bool bind_to(const unsigned int bind_port);

         void shutdown_sockets(void);

         int socket_handle_;

         bool bound_;

         unsigned int bind_port_;
   };
}

#endif
