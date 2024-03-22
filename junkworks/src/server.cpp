#include "junkworks/server.hpp"

#include "junkworks/network_common.h"

#include <iostream>

namespace junkworks
{
   Server::Server(const unsigned int port)
      : socket_(port)
      , invalid_uid_(-1)
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

      // Clear out the old stuff that was received
      for (auto & uid_to_rx : uid_to_rx_bytes_)
      {
         uid_to_rx.second.clear();
      }

      // Add the new stuff
      for (auto & behavior_packet : rx_behavior_packets_)
      {
         byte_data_t<128> temp_data;
         const unsigned int uid = get_uid(behavior_packet);
         if (uid == invalid_uid_)
         {
            continue;
         }

         temp_data = behavior_packet;

         uid_to_rx_bytes_.at(uid).push_back(temp_data);
      }

      // Send any new stuff
      for (const auto & uid_to_tx: uid_to_tx_bytes_)
      {
         const client_connection_t connection = get_connection(
            uid_to_tx.first
         );

         if (connection.ip == 0)
         {
            continue;
         }

         for (const auto & byte_pack : uid_to_tx.second)
         {
            socket_.try_send(
               connection.ip, connection.port, byte_pack.data, byte_pack.size
            );
         }
      }

      // Delete what you just sent
      for (auto & uid_to_tx : uid_to_tx_bytes_)
      {
         uid_to_tx.second.clear();
      }
   }

   void Server::handle_handshake(const raw_rx_payload_t<128> & payload)
   {
      // oh my this is lazy
      unsigned int port = 8000 + static_cast<unsigned char>(payload[1]);
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

      std::cout << "New client connection! Sending new client UID\n";
      const unsigned int uid = connections_.size();
      connections_[conn] = uid;

      uid_to_rx_bytes_[uid];
      uid_to_tx_bytes_[uid];

      send_affirmative_handshake(payload.sender_ip, port, uid);
   }

   std::vector<byte_data_t<128> > Server::get_client_rx_bytes(
      const unsigned int uid
   ) const
   {
      std::vector<byte_data_t<128> > client_bytes_copy;
      auto client_rx_bytes_iter = uid_to_rx_bytes_.find(uid);
      if (client_rx_bytes_iter == uid_to_rx_bytes_.end())
      {
         std::cout << "Trying to grab data from non-existent UID, " << uid << "\n";
         return client_bytes_copy;
      }

      if (client_rx_bytes_iter->second.size() == 0)
      {
         return client_bytes_copy;
      }

      client_bytes_copy.reserve(client_rx_bytes_iter->second.size());
      client_bytes_copy = client_rx_bytes_iter->second;

      return client_bytes_copy;
   }

   void Server::set_client_tx_bytes(
      const unsigned int uid, const std::vector<byte_data_t<128> > & tx_bytes
   )
   {
      auto client_tx_bytes_iter = uid_to_tx_bytes_.find(uid);
      if (client_tx_bytes_iter == uid_to_tx_bytes_.end())
      {
         std::cout << "Trying to send data to non-existent UID, " << uid << "\n";
         return;
      }

      client_tx_bytes_iter->second = tx_bytes;
   }

   std::vector<unsigned int> Server::get_uids(void) const
   {
      std::vector<unsigned int> uids;
      for (const auto & conn_uid : connections_)
      {
         uids.push_back(conn_uid.second);
      }

      return uids;
   }

   unsigned int Server::get_uid(const raw_rx_payload_t<128> & packet) const
   {
      unsigned int port = 8000 + static_cast<unsigned char>(packet[1]);
      client_connection_t temp_connection(packet.sender_ip, port);

      const auto connect_uid_iter = connections_.find(temp_connection);

      if (connect_uid_iter == connections_.end())
      {
         return invalid_uid_;
      }

      return connect_uid_iter->second;
   }

   client_connection_t Server::get_connection(const unsigned int uid) const
   {
      // ooooo I can do better than this
      for (const auto & connect_uid : connections_)
      {
         if (connect_uid.second == uid)
         {
            return connect_uid.first;
         }
      }

      client_connection_t nonexistent_connection(0, 0);
      return nonexistent_connection;
   }

   void Server::send_negative_handshake(
      const unsigned int addr, const unsigned int port
   )
   {
      unsigned char message[128];
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
      unsigned char message[128];
      message[0] = 1;
      message[1] = 0;
      message[2] = (uid << 0) >> 24;
      message[3] = (uid << 8) >> 24;
      message[4] = (uid << 16) >> 24;
      message[5] = (uid << 24) >> 24;

      socket_.try_send(ipv4add(ntohl(addr)), port, message, 128);
   }
}
