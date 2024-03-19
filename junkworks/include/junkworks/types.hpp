#ifndef JUNKWORKS_TYPES_HEADER
#define JUNKWORKS_TYPES_HEADER

namespace junkworks
{
   template <int Size_T>
   struct raw_payload_t
   {
      char data[Size_T];
      int size;
      unsigned int ip;

      char & operator[](int i)
      {
         return data[i];
      }

      const char & operator[](int i) const
      {
         return data[i];
      }
   };
}

#endif
