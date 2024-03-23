#include "junkworks/server.hpp"

#include <chrono>
#include <iostream>
#include <thread>

int bytes_to_int(const junkworks::byte_data_t<128> & bytes)
{
   int val = 0;
   for (int i = 0; i < 4; ++i)
   {
      int temp = bytes[2 + i];
      val += temp << (i * 8);
   }

   return val;
}

int main(int argc, char ** argv)
{
   junkworks::Server server(8123);

   return 0;
}
