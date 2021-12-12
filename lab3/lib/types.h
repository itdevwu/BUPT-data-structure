#include "./io.h"

namespace lab3
{
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

        Bitseq()
        {
            size = 0;
            data = 0;
        }
    };

    struct Freq
    {
        lab3::UTF8_CHAR content;
        int frequency;

        Freq()
        {
            frequency = -1;
        }
    };
}