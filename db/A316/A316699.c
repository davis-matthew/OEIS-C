/*
 * A316699  Number of (marked) cyclic n-bit binary strings containing no runs
 *          of length > 3.
 * https://oeis.org/A316699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-3) + 3*a(n-4) + 2*a(n-5) + a(n-6), with
 *            a(1)=2,a(2)=4,a(3)=8,a(4)=14,a(5)=20,a(6)=38,a(7)=70,a(8)=134,a(9)=240
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t a_table[10] = {0, 2, 4, 8, 14, 20, 38, 70, 134, 240};

seq_t a(idx_t n)
{
    if (n <= 9)
        return a_table[(size_t)n];

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0; /* highest computed index in cache */

    size_t index = (size_t)n;
    if (len < index)
    {
        /* grow capacity */
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 16;
        while (new_cap < index + 1)
            new_cap <<= 1;
        if (cache == NULL)
        {
            cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (!cache) exit(EXIT_FAILURE);
        }
        else
        {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
        }
        cap = new_cap;

        /* initialize base values if not yet done */
        if (len == 0)
        {
            for (size_t i = 1; i <= 9; ++i)
                cache[i] = a_table[i];
            len = 9;
        }
        else if (len < 9)
        {
            for (size_t i = len + 1; i <= 9; ++i)
                cache[i] = a_table[i];
            len = 9;
        }

        for (size_t i = len + 1; i <= index; ++i)
        {
            cache[i] =
                  cache[i-2]
                + ((seq_t)2 * cache[i-3])
                + ((seq_t)3 * cache[i-4])
                + ((seq_t)2 * cache[i-5])
                + cache[i-6];
        }
        len = index;
    }
    return cache[index];
}
