/*
 * A255595  Sylvester's sequence modulo 109.
 * https://oeis.org/A255595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2; a(n) = a(n-1)^2 - a(n-1) + 1 (mod 109)
 * Range:     u8 n <= 3, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache: a[0..cached-1] already computed */
    static seq_t *cache = NULL;
    static size_t cached = 0;          /* number of cached entries */
    const seq_t MOD = (seq_t)109;      /* modulus */

    /* Expand cache if needed */
    if ((size_t)n >= cached)
    {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp;

        if (!cache)
            tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        else
            tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        if (!tmp)                /* allocation failed */
            exit(1);

        cache = tmp;

        /* Compute missing terms sequentially */
        for (size_t i = cached; i <= (size_t)n; ++i)
        {
            if (i == 0)
                cache[0] = (seq_t)2;
            else
            {
                seq_t prev = cache[i - 1];
                /* Since prev < 109, prev*prev < 11900, safe */
                seq_t val = prev * prev;
                val = (val - prev + 1 + MOD) % MOD;
                cache[i] = val;
            }
        }

        cached = new_size;
    }

    return cache[(size_t)n];
}
