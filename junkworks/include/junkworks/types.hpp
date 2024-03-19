#ifndef JUNKWORKS_TYPES_HEADER
#define JUNKWORKS_TYPES_HEADER

namespace junkworks
{
   template <int Size_T>
   struct raw_payload_t
   {
      char data[Size_T];
      int size;
      unsigned int ip;

      char & operator[](int i)
      {
         return data[i];
      }

      const char & operator[](int i) const
      {
         return data[i];
      }
   };

   struct client_connection_t
   {
      unsigned int ip;
      unsigned int port;

      client_connection_t(
         const unsigned int ip, const unsigned int port
      )
         : ip(ip)
         , port(port)
      { }

      client_connection_t(void) = default;

      client_connection_t(const client_connection_t &) = default;

      bool operator==(const client_connection_t & that) const
      {
         return ip == that.ip && port == that.port;
      };

      bool operator<(const client_connection_t & that) const
      {
         return ip < that.ip && port < that.port;
      }
   };
}

#endif
