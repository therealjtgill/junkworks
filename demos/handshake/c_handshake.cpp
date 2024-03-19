#include "junkworks/client.hpp"

#include <chrono>
#include <cstdlib>
#include <thread>

int main(int argc, char ** argv)
{
   if (argc < 5)
   {
      return 1;
   }

   junkworks::ipv4add server_ip(
      std::atoi(argv[1]),
      std::atoi(argv[2]),
      std::atoi(argv[3]),
      std::atoi(argv[4])
   );
   junkworks::Client client(8222, server_ip, 8123);

   while(client.handshake_in_progress())
   {
      client.update();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
   }

   return 0;
}
