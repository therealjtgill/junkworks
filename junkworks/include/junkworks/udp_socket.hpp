#ifndef UDP_SOCKET_HEADER
#define UDP_SOCKET_HEADER

#include "junkworks/network_common.h"

struct Ipv4Address
{
   Ipv4Address(
      const unsigned char o1,
      const unsigned char o2,
      const unsigned char o3,
      const unsigned char o4
   )
      : ip{o1, o2, o3, o4}
   { }

   Ipv4Address(void) = delete;

   // Returns a number that can be inserted directly into
   // 'sockaddr_in::sin_addr::s_addr'
   unsigned int internet_address(void) const
   {
      unsigned int temp_ip = (
         (ip[0] << 24)
         | (ip[1] << 16)
         | (ip[2] << 8)
         | ip[3]
      );

      return htonl(temp_ip);
   }

   unsigned int & operator[](const unsigned int i)
   {
      return ip[i];
   }

   const unsigned int & operator[](const unsigned int i) const
   {
      return ip[i];
   }

   private:
      unsigned int ip[4];
};

class UdpSocket
{
   public:
      UdpSocket(const unsigned int bind_port);

      ~UdpSocket(void);

      bool bind_to(const unsigned int bind_port);

      bool try_send(
         const Ipv4Address dest_ip,
         const unsigned int dest_port,
         const char * data,
         const unsigned int data_len
      ) const;

      int try_receive(
         const unsigned int max_data_len,
         char * data
      ) const;

      bool bound(void) const;

   private:
      bool initialize_sockets(void);

      void shutdown_sockets(void);

      int socket_handle_;

      bool bound_;

      unsigned int bind_port_;
};

#endif
