#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <locale.h>
#include <wchar.h>

#include <bitset>
#include <iostream>

namespace lab3
{
    typedef wint_t UTF8_CHAR;

    struct BitIO
    {
        FILE *target_file_pointer;
        int current_bit;
        unsigned char bit_buffer;

        void flush()
        {
            while (current_bit)
                write_bit(0);
        }

        void write_bit(int bit)
        {
            if (bit)
                bit_buffer |= ((unsigned char)1 << current_bit);

            current_bit++;
            if (current_bit == 8)
            {
                // puts(" Block");
                fwrite(&bit_buffer, sizeof(unsigned char), 1, target_file_pointer);
                current_bit = 0;
                bit_buffer = 0;
            }
        }

        BitIO()
        {
            current_bit = 0;
            bit_buffer = 0;
        }
    };
}