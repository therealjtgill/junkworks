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
      rx_packets_.clear();
      socket_.receive_all(rx_packets_);

      rx_handshake_packets_.clear();
      rx_behavior_packets_.clear();

      for (auto & packet : rx_packets_)
      {
         const uint8_t packet_type = packet[0];
         if (packet_type == 1)
         {
            rx_handshake_packets_.push_back(packet);
         }
         else
         {
            rx_behavior_packets_.push_back(packet);
         }
      }

      for (auto & handshake_packet : rx_handshake_packets_)
      {
         handle_handshake(handshake_packet);
      }

      for (auto & behavior_packet : rx_behavior_packets_)
      {
         // or just give these to the behavior wholesale
      }
   }

   void Server::handle_handshake(const raw_rx_payload_t<128> & payload)
   {
      // oh my this is lazy
      unsigned int port = 8000 + static_cast<unsigned char>(payload.data[1]);
      std::cout << "Received handshake port: " << port << "\n";
      std::cout << "Received handshake ip: " << ntohl(payload.sender_ip) << "\n";

      ipv4add temp_ip(ntohl(payload.sender_ip));
      std::cout << "Handshake ip: ";
      for (int i = 0; i < 4; ++i)
      {
         std::cout << temp_ip[i];
         if (i < 3)
         {
            std::cout << ".";
         }
      }
      std::cout << "\n";

      if (connections_.size() == 4)
      {
         send_negative_handshake(payload.sender_ip, port);

         return;
      }

      client_connection_t conn(payload.sender_ip, port);

      const auto connection_iter = connections_.find(conn);

      if (connection_iter != connections_.end())
      {
         std::cout << "Connection to client exists, sending existing client UID\n";
         send_affirmative_handshake(
            payload.sender_ip, port, connection_iter->second
         );
         return;
      }

      std::cout << "New client connection! sending new client UID\n";
      const unsigned int uid = connections_.size();
      connections_[conn] = uid;
      send_affirmative_handshake(payload.sender_ip, port, uid);
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
      message[2] = (uid << 0) >> 24;
      message[3] = (uid << 8) >> 24;
      message[4] = (uid << 16) >> 24;
      message[5] = (uid << 24) >> 24;

      socket_.try_send(ipv4add(ntohl(addr)), port, message, 128);
   }
}
