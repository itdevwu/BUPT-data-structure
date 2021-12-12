#include <bits/stdc++.h>

namespace lab3
{

#define ALPHABET_LEN 26
#define NOT_FOUND -1
#define max(a, b) (a) > (b) ? (a) : (b)

    int is_prefix(unsigned char *word, int wordlen, int pos)
    {
        int i;
        int suffixlen = wordlen - pos;

        for (i = 0; i < suffixlen; i++)
        {
            if (word[i] != word[pos + i])
            {
                return 0;
            }
        }
        return 1;
    }

    int suffix_length(unsigned char *word, int wordlen, int pos)
    {
        int i;
        // increment suffix length i to the first mismatch or beginning
        // of the word
        for (i = 0; (word[pos - i] == word[wordlen - 1 - i]) && (i < pos); i++)
            ;
        return i;
    }

    void make_delta1(int *delta1, unsigned char *pat, unsigned patlen)
    {
        int i;

        for (i = 0; i < ALPHABET_LEN; i++)
        {
            delta1[i] = NOT_FOUND;
        }
        for (i = 0; i < patlen - 1; i++)
        {
            delta1[pat[i]] = patlen - 1 - i;
        }
    }

    void make_delta2(int *delta2, unsigned char *pat, int patlen)
    {
        int p;
        int last_prefix_index = 1;

        // first loop, prefix pattern
        for (p = patlen - 1; p >= 0; p--)
        {
            if (is_prefix(pat, patlen, p + 1))
            {
                last_prefix_index = p + 1;
            }
            delta2[p] = (patlen - 1 - p) + last_prefix_index;
        }

        // this is overly cautious, but will not produce anything wrong
        // second loop, suffix pattern
        for (p = 0; p < patlen - 1; p++)
        {
            int slen = suffix_length(pat, patlen, p);
            if (pat[p - slen] != pat[patlen - 1 - slen])
            {
                delta2[patlen - 1 - slen] = patlen - 1 - p + slen;
            }
        }
    }

    int boyer_moore(unsigned char *string,
                         unsigned stringlen,
                         unsigned char *pat,
                         unsigned patlen,
                         unsigned max_alphabet_len)
    {
        int *delta1 = (int *)malloc(max_alphabet_len * sizeof(int));
        int *delta2 = (int *)malloc(patlen * sizeof(int));
        make_delta1(delta1, pat, patlen);
        make_delta2(delta2, pat, patlen);
        int n_shifts = 0;
        int chars_compared = 0;
        int i = patlen - 1;
        while (i < stringlen)
        {
            int j = patlen - 1;
            while (j >= 0 && (string[i] == pat[j]))
            {
                --i, --j;
                ++chars_compared;
            }
            if (j < 0)
            {
                free(delta1);
                free(delta2);
                return (unsigned)i + 1;
            }
            ++chars_compared;

            i += max(delta1[string[i]], delta2[j]);
        }

        free(delta1);
        free(delta2);
        return -1;
    }
}