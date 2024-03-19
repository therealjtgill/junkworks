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
   {
      if (bind_port < 8000)
      {
         std::cout << "bind port must be greater than 8000!\n";
         std::cout << "i'm not going to do anything about it, though. it's your foot\n";
      }
   }

   void Client::update(void)
   {
      packets_.clear();
      socket_.receive_all(packets_);

      if (!packets_.empty())
      {
         std::cout << "got " << packets_.size() << " packets\n";
      }

      if (handshake_in_progress_)
      {
         handshake();
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
      for (auto & packet : packets_)
      {
         const uint8_t packet_type = packet[0];
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
               std::cout << "connection rejected :(\n";
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
                  std::cout << "uid byte " << i << ": " << uid_byte << "\n";
                  uid_byte <<= ((3 - i) * 8);
                  uid_ += uid_byte;
               }

               handshake_in_progress_ = false;
               got_response = true;
               std::cout << "connection accepted! uid: " << uid_ << "\n";
            }
         }

         if (!got_response)
         {
            no_ack_counter_ += 1;

            if (no_ack_counter_ > max_handshake_attempts_)
            {
               std::cout << "Couldn't make connection\n";
               handshake_in_progress_ = false;
            }

            if (no_ack_counter_ % 5 == 0)
            {
               send_handshake_packet();
            }
         }
      }
   }

   void Client::send_handshake_packet(void)
   {
      char data[128];
      data[0] = 1;
      data[1] = socket_.bind_port() - 8000;
      socket_.try_send(server_ip_, server_port_, data, 128);
   }
}
