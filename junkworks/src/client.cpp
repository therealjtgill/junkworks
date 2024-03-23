#include "junkworks/client.hpp"

#include <iostream>

namespace junkworks
{
   Client::Client(
      const unsigned int bind_port,
      const ipv4add server_ip,
      const unsigned int server_port
   )
      : socket_(bind_port)
      , server_ip_(server_ip)
      , server_port_(server_port)
      , max_handshake_attempts_(16)
      , no_ack_counter_(0)
      , handshake_in_progress_(true)
      , first_handshake_sent_(false)
      , uid_(-1)
      , behavior_running_(false)
   {
      if (bind_port < 8000)
      {
         std::cout << "bind port must be greater than 8000!\n";
         std::cout << "i'm not going to do anything about it, though. it's your foot\n";
      }
   }

   void Client::update(void)
   {
      rx_packets_.clear();
      socket_.receive_all(rx_packets_);

      if (handshake_in_progress_)
      {
         handshake();
      }
      else if (!handshake_in_progress_ && behavior_running_)
      {
         rx_bytes_.clear();
         byte_data_t<128> temp_byte_data;
         for (const auto & rx_packet : rx_packets_)
         {
            temp_byte_data = rx_packet;
            rx_bytes_.push_back(temp_byte_data);
         }

         for (const auto & tx_msg : tx_bytes_)
         {
            socket_.try_send(
               server_ip_, server_port_, tx_msg.data, 128
            );
         }

         tx_bytes_.clear();
      }
   }

   void Client::handshake(void)
   {
      if (!first_handshake_sent_)
      {
         send_handshake_packet();
         first_handshake_sent_ = true;
      }

      bool got_response = false;
      for (auto & packet : rx_packets_)
      {
         const char packet_type = packet[0];
         if (packet_type == 1)
         {
            if (
               (packet[0] == 1)
               && (packet[1] == 0)
               && (packet[2] == 123)
               && (packet[3] == 123)
            )
            {
               handshake_in_progress_ = false;
               got_response = true;
               std::cout << "Connection to server rejected :(\n";
            }
            else if (
               (packet[0] == 1)
               && (packet[1] == 0)
            )
            {
               uid_ = 0;
               for (int i = 0; i < 4; ++i)
               {
                  unsigned int uid_byte = static_cast<unsigned char>(packet[2 + i]);
                  uid_byte <<= ((3 - i) * 8);
                  uid_ += uid_byte;
               }

               handshake_in_progress_ = false;
               got_response = true;
               std::cout << "Connection accepted! UID: " << uid_ << "\n";
            }
         }
         else
         {
            std::cout << "Unhandled packet type\n";
         }
      }

      if (!got_response)
      {
         no_ack_counter_ += 1;

         if (no_ack_counter_ > max_handshake_attempts_)
         {
            std::cout << "Couldn't make connection to server\n";
            handshake_in_progress_ = false;
         }

         if (no_ack_counter_ % 5 == 0)
         {
            send_handshake_packet();
         }
      }
      else
      {
         behavior_running_ = true;
      }
   }

   std::vector<byte_data_t<128> > Client::get_rx_bytes(void) const
   {
      std::vector<byte_data_t<128> > bytes_copy;
      bytes_copy.reserve(rx_bytes_.size());
      bytes_copy = rx_bytes_;

      return bytes_copy;
   }

   void Client::set_tx_bytes(const std::vector<byte_data_t<128> > & tx_bytes)
   {
      tx_bytes_ = tx_bytes;
   }

   void Client::send_handshake_packet(void)
   {
      unsigned char data[128];
      data[0] = 1;
      data[1] = socket_.bind_port() - 8000;
      socket_.try_send(server_ip_, server_port_, data, 128);
   }
}
