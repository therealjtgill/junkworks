#include "junkworks/server.hpp"

#include <chrono>
#include <thread>

int main(void)
{
   junkworks::Server server(8123);

   while(true)
   {
      server.update();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
   }
}
