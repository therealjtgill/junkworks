#include "junkworks/server.hpp"

#include "junkworks/network_common.h"

#include <iostream>

namespace junkworks
{
   Server::Server(const unsigned int port)
      : socket_(port)
   { }

   void Server::update(void)
   {
      packets_.clear();
      socket_.receive_all(packets_);

      std::cout << "got " << packets_.size() << " packets\n";

      for (auto & packet : packets_)
      {
         const uint8_t packet_type = packet[0];
         if (packet_type == 1)
         {
            handle_handshake(packet);
         }
      }
   }

   void Server::handle_handshake(const raw_payload_t<128> & payload)
   {
      // oh my this is lazy
      unsigned int port = 8000 + payload.data[1];
      if (connections_.size() == 4)
      {
         send_negative_handshake(
            payload.ip, port
         );

         return;
      }

      client_connection_t conn(payload.ip, port);

      const auto connection_iter = connections_.find(conn);

      if (connection_iter != connections_.end())
      {
         send_affirmative_handshake(
            payload.ip, port, connection_iter->second
         );
      }

      const unsigned int uid = connections_.size();
      connections_[conn] = uid;
      send_affirmative_handshake(payload.ip, port, uid);
   }

   void Server::send_negative_handshake(
      const unsigned int addr, const unsigned int port
   )
   {
      char message[128];
      message[0] = 1;
      message[1] = 0;
      message[2] = 123;
      message[3] = 123;

      socket_.try_send(ipv4add(ntohl(addr)), port, message, 128);
   }

   void Server::send_affirmative_handshake(
      const unsigned int addr,
      const unsigned int port,
      const unsigned int uid
   )
   {
      char message[128];
      message[0] = 1;
      message[1] = 0;
      message[2] = (uid << 24) >> 24;
      message[3] = (uid << 16) >> 24;
      message[4] = (uid << 8) >> 24;
      message[5] = (uid << 0) >> 24;

      socket_.try_send(ipv4add(ntohl(addr)), port, message, 128);
   }
}
