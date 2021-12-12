#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <locale.h>
#include <wchar.h>

#include <iostream>

namespace lab3
{
    // UTF-8 char.
    typedef wint_t UTF8_CHAR;

    // Control bits' writing.
    struct BitIO
    {
        // Target file.
        FILE *target_file_pointer;
        
        // Buffer and its current size.
        int current_bit;
        unsigned char bit_buffer;

        // Add suffix 0s.
        void flush()
        {
            while (current_bit)
                write_bit(0);
        }

        // Write a bit.
        void write_bit(int bit)
        {
            // No need to write if it's 0.
            if (bit)
                bit_buffer |= ((unsigned char)1 << current_bit);

            current_bit++;

            // Buffer is full.
            if (current_bit == 8)
            {
                // puts(" Block");
                fwrite(&bit_buffer, sizeof(unsigned char), 1, target_file_pointer);
                current_bit = 0;
                bit_buffer = 0;
            }
        }

        // Init.
        BitIO()
        {
            current_bit = 0;
            bit_buffer = 0;
        }
    };
}