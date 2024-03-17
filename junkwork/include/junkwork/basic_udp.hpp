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

struct Ipv4Address
{
   Ipv4Address(
      const unsigned char o1,
      const unsigned char o2,
      const unsigned char o3,
      const unsigned char o4
   )
      : ip{o1, o2, o3, o4}
   { }

   Ipv4Address(void) = delete;

   // Returns a number that can be inserted directly into
   // 'sockaddr_in::sin_addr::s_addr'
   unsigned int internet_address(void) const
   {
      unsigned int temp_ip = (
         (ip[0] << 24)
         | (ip[1] << 16)
         | (ip[2] << 8)
         | ip[3]
      );

      return htonl(temp_ip);
   }

   unsigned int & operator[](const unsigned int i)
   {
      return ip[i];
   }

   const unsigned int & operator[](const unsigned int i) const
   {
      return ip[i];
   }

   private:
      unsigned int ip[4];
};

bool initialize_sockets(void);

void shutdown_sockets(void);

int create_socket(void);

bool bind_socket(const int handle, const unsigned int port);

void close_socket(
   const int handle
);

bool send_packet(
   const int handle,
   const sockaddr_in to_address,
   const char * data,
   const unsigned int packet_size
);

bool receive_packet(
   const int socket,
   const unsigned int max_packet_size,
   sockaddr_in * from_address,
   char * packet_data
);
