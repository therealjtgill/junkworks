#ifndef JUNKWORKS_TYPES_HEADER
#define JUNKWORKS_TYPES_HEADER

#include <cstring>

namespace junkworks
{
   template <int Size_T>
   struct raw_rx_payload_t
   {
      char data[Size_T];
      int size;
      unsigned int sender_ip;

      char & operator[](int i)
      {
         return data[i];
      }

      const char & operator[](int i) const
      {
         return data[i];
      }
   };

   template <int Size_T>
   struct raw_tx_payload_t
   {
      char data[Size_T];
      int size;

      char & operator[](int i)
      {
         return data[i];
      }

      const char & operator[](int i) const
      {
         return data[i];
      }
   };

   template <int Size_T>
   struct byte_data_t
   {
      char data[Size_T];
      int size;

      char & operator[](int i)
      {
         return data[i];
      }

      const char & operator[](int i) const
      {
         return data[i];
      }

      byte_data_t & operator=(const raw_tx_payload_t<Size_T> & tx_payload)
      {
         for (int i = 0; i < Size_T; ++i)
         {
            data[i] = tx_payload[i];
            size = tx_payload.size;
         }

         return *this;
      }

      byte_data_t & operator=(const raw_rx_payload_t<Size_T> & rx_payload)
      {
         for (int i = 0; i < Size_T; ++i)
         {
            data[i] = rx_payload[i];
            size = rx_payload.size;
         }

         return *this;
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
         return (ip == that.ip) && (port == that.port);
      };

      bool operator<(const client_connection_t & that) const
      {
         // return (ip < that.ip) && (port < that.port);
         if (ip < that.ip)
         {
            return true;
         }
         if (ip > that.ip)
         {
            return false;
         }

         return port < that.port;
      }
   };
}

#endif
