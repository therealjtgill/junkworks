#ifndef NETWORK_COMMON_HEADER
#define NETWORK_COMMON_HEADER

#ifdef __cplusplus
extern "C"
{
#endif

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

#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>

#endif

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

#ifdef __cplusplus
}
#endif

#endif
