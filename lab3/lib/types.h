#include "./io.h"

namespace lab3
{
    // Record encoding of a utf-8 char.
    struct Bitseq
    {
        lab3::UTF8_CHAR ch;
        int size;
        int data;

        // Return the n-th bit, starting from 0.
        int nth_bit(int n)
        {
            if (n >= size)
                return -1;
            return (data >> n) & 1;
        }

        // Init.
        Bitseq()
        {
            size = 0;
            data = 0;
        }
    };

    // Record frequency of a UTF-8 char.
    struct Freq
    {
        lab3::UTF8_CHAR content;
        int frequency;

        // -1 means no such char.
        Freq()
        {
            frequency = -1;
        }
    };
}