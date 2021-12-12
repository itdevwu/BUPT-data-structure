#include <bits/stdc++.h>

namespace lab3
{

// 2 is enough, 26 is for debug.
#define ALPHABET_LEN 26
#define NOT_FOUND -1
#define max(a, b) (a) > (b) ? (a) : (b)

    int is_prefix(unsigned char *str, int string_length, int pos)
    {
        int i;
        int suffixlen = string_length - pos;

        for (i = 0; i < suffixlen; ++i)
        {
            if (str[i] != str[pos + i])
            {
                return 0;
            }
        }
        return 1;
    }

    int suffix_length(unsigned char *str, int string_length, int pos)
    {
        int i;
        // Increment suffix length i to the first mismatch or beginning of the str.
        for (i = 0; (str[pos - i] == str[string_length - 1 - i]) && (i < pos); ++i)
            ;
        return i;
    }

    void make_delta1(int *delta1, unsigned char *pattern, unsigned pattern_length)
    {
        for (int i = 0; i < ALPHABET_LEN; ++i)
        {
            delta1[i] = NOT_FOUND;
        }
        for (int i = 0; i < pattern_length - 1; ++i)
        {
            delta1[pattern[i]] = pattern_length - 1 - i;
        }
    }

    void make_delta2(int *delta2, unsigned char *pattern, int pattern_length)
    {
        int p;
        int last_prefix_index = 1;

        // First loop, prefix pattern.
        for (p = pattern_length - 1; p >= 0; p--)
        {
            if (is_prefix(pattern, pattern_length, p + 1))
            {
                last_prefix_index = p + 1;
            }
            delta2[p] = (pattern_length - 1 - p) + last_prefix_index;
        }

        // Be cautious avoiding produce anything wrong, second loop for suffix pattern.
        for (p = 0; p < pattern_length - 1; p++)
        {
            int slen = suffix_length(pattern, pattern_length, p);
            if (pattern[p - slen] != pattern[pattern_length - 1 - slen])
            {
                delta2[pattern_length - 1 - slen] = pattern_length - 1 - p + slen;
            }
        }
    }

    // Match string using Boyer-Moore.
    // Returning position.
    int boyer_moore(unsigned char *string,
                         unsigned stringlen,
                         unsigned char *pat,
                         unsigned patlen,
                         unsigned max_alphabet_len)
    {
        // Generate d1, d2.
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
                // Matched.
                free(delta1);
                free(delta2);
                return (unsigned)i + 1;
            }
            ++chars_compared;

            i += max(delta1[string[i]], delta2[j]);
        }

        free(delta1);
        free(delta2);
        // Without match.
        return -1;
    }
}