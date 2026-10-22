/*
 * A301137  Coordination sequence T1 for Zeolite Code NAB.
 * https://oeis.org/A301137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-3) - 3*a(n-6) + a(n-9) + c_n * where c_n are the
 *            coefficients of the numerator of the generating * function:
 *            {1,4,9,16,28,28,26,7,0,-1,0,2} for 0≤n≤11, and 0 for n>11. *
 *            Initial values: a(0)=1, a(1)=4, a(2)=9
 * Range:     u8 n <= 10, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int coeff[12] = {1,4,9,16,28,28,26,7,0,-1,0,2};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n < cache_len)
        return cache[(size_t)n];

    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    for (size_t i = cache_len; i <= n; ++i)
    {
        seq_t val = 0;

        if (i >= 3)
            val += cache[i - 3] * (seq_t)3;
        if (i >= 6)
            val -= cache[i - 6] * (seq_t)3;
        if (i >= 9)
            val += cache[i - 9];

        if (i < 12) {
            int c = coeff[i];
            if (c > 0)
                val += (seq_t)c;
            else if (c < 0)
                val -= (seq_t)(-c);
        }

        cache[i] = val;
    }

    cache_len = new_size;
    return cache[(size_t)n];
}
