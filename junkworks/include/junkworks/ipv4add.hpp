#ifndef IPV4ADD_HEADER
#define IPV4ADD_HEADER

namespace junkworks
{
   struct ipv4add
   {
      ipv4add(
         const unsigned char o1,
         const unsigned char o2,
         const unsigned char o3,
         const unsigned char o4
      );

      ipv4add(const unsigned int ip);

      ipv4add(void) = delete;

      // Returns a number that can be inserted directly into
      // 'sockaddr_in::sin_addr::s_addr'
      unsigned int internet_address(void) const;

      unsigned int & operator[](const unsigned int i);

      const unsigned int & operator[](const unsigned int i) const;

      private:
         unsigned int ip[4];
   };
}

#endif
