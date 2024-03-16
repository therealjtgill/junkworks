#define PLATFORM_WINDOWS 1
#define PLATFORM_MAC     2
#define PLATFORM_UNIX    3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>

#elif PLATFORM == PLATFORM_MAC || \
      PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#endif

#include <iostream>

bool initialize_sockets(void)
{
#if PLATFORM == PLATFORM_WINDOWS
   WSADATA wsa_data;
   return WSAStartup( MAKEWORD(2,2), &wsa_data) == NO_ERROR;
#else
   return true;
#endif
}

void shutdown_sockets(void)
{
#if PLATFORM == PLATFORM_WINDOWS
   WSACleanup();
#endif
}

int create_socket(void)
{
   // create the socket
   int handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (handle <= 0)
   {
      std::cout << "Failed to create socket\n";
   }

   return handle;
}

bool bind_socket(const int handle, const unsigned int port)
{
   // htons = host to network short. used to convert host byte order to
   // network byte order. required whenever you set integer members in socket
   // structures.
   // htonl = host to network long.
   sockaddr_in address;
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons( static_cast<const unsigned short>(port));

   if (
      bind(
         handle,
         // static_cast<const sockaddr*>(&address),
         (const sockaddr*)&address,
         sizeof(sockaddr_in)
      ) < 0
   )
   {
      std::cout << "Failed to bind socket\n";
      return false;
   }

   // set socket as non-blocking
#if PLATFORM == PLATFORM_MAC || \
    PLATFORM == PLATFORM_UNIX
   int non_blocking = 1;
   if (
      fcntl(
         handle, F_SETFL, O_NONBLOCK, non_blocking
      ) == 1
   )
   {
      std::cout << "Failed to set port as non-blocking\n";
      return false;
   }
#elif
   DWORD non_blocking = 1;
   if (
      isoctlsocket(
         handle, FIONBIO, &non_blocking
      ) != 0
   )
   {
      std::cout << "Failed to set port as non-blocking\n";
      return false;
   }
#endif

   return true;
}

void close_socket(
   const int handle
)
{
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
   close(handle);
#elif PLATFORM == PLATFORM_WINDOWS
   closesocket(handle);
#endif
}

bool send_packet(
   const int handle,
   const sockaddr_in to_address,
   const char * data,
   const unsigned int packet_size
)
{
   const unsigned int sent_bytes = sendto(
      handle,
      data,
      packet_size,
      0,
      (sockaddr*)&to_address,
      sizeof(sockaddr_in)
   );

   if (sent_bytes != packet_size)
   {
      std::cout << "Failed to send packet\n";
   }

   return (sent_bytes != packet_size);
}

bool receive_packet(
   const int socket,
   const unsigned int max_packet_size,
   sockaddr_in * from_address,
   char * packet_data
)
{
   // any packet larger than 'max_packet_size' will be dropped
   unsigned int from_address_size = sizeof(from_address);
   int bytes = recvfrom(
      socket,
      packet_data,
      max_packet_size,
      0,
      (sockaddr*)from_address,
      &from_address_size
   );

   return bytes > 0;
}

int main(int argc, char ** argv)
{
   if (argc < 2)
   {
      return 0;
   }

   // create the socket
   int handle = create_socket();

   // // bind the socket
   // unsigned int port = 8123;

   unsigned int bind_port = atoi(argv[1]);

   std::cout << "port: " << bind_port << "\n";

   if (!bind_socket(handle, bind_port))
   {
      return 1;
   }

   unsigned int a = 127;
   unsigned int b = 0;
   unsigned int c = 0;
   unsigned int d = 1;

   unsigned int ip_add = (a << 24) | (b << 16) | ( c << 8) | d;

   sockaddr_in send_address;
   send_address.sin_family = AF_INET;
   send_address.sin_addr.s_addr = htonl(ip_add);
   send_address.sin_port = htons(8123);

   send_packet(handle, send_address, "stuff", sizeof("stuff"));

   if (bind_port != 8123)
   {
      return 0;
   }

   sockaddr_in from_address;
   char packet_data[6];

   while(true)
   {
      // if (receive_packet(handle, 6, &from_address, packet_data))
      if (receive_packet(handle, 6, &from_address, packet_data))
      {
         std::cout << "packet: " << packet_data << "\n";
      }
   }

   // std::cout << "got packet\n";

   // for (int i = 0; i < 5; ++i)
   // {
   //    std::cout << static_cast<int>(packet_data[i]) << " ";
   // }
   // std::cout << "\n";

   return 0;
}
