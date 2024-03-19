#include "junkworks/client.hpp"

#include <chrono>
#include <thread>

int main(void)
{
   junkworks::ipv4add server_ip(127, 0, 0, 1);
   junkworks::Client client(8222, server_ip, 8123);

   while(client.handshake_in_progress())
   {
      client.update();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
   }
}
