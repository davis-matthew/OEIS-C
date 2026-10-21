/*
 * A277548  Numbers k such that k/5^m == 4 (mod 5), where 5^m is the greatest
 *          power of 5 that divides k.
 * https://oeis.org/A277548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277548: numbers k such that k / 5^m ≡ 4 (mod 5), where m is the largest power of 5 dividing k. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset starts at 1 */

    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 0;

    while (cache_len < (size_t)n)
    {
        seq_t k = last_checked + 1;

        /* find next k satisfying the property */
        while (1)
        {
            seq_t y = k;
            while (y % (seq_t)5 == 0)
                y /= (seq_t)5;

            if (y % (seq_t)5 == (seq_t)4)
            {
                if (cache_len == cache_cap)
                {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                last_checked = k;
                break;
            }
            ++k;
        }
    }

    return cache[n - 1];
}
