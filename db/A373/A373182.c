/*
 * A373182  Number of ways that people can sit in n linearly arranged seats
 *          such that there are one or two empty seats between any two
 *          persons, zero or one empty seats at the start and end, and at
 *          least one person gets seated.
 * https://oeis.org/A373182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n-1)*a(n-4) + 2*n*a(n-3) + (n+1)*a(n-2) - 3*a(n-1))/2
 *            for n>4 Initial values: a(1)=1, a(2)=2, a(3)=3, a(4)=6 (OEIS
 *            offset: 1)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i+1) for i >= 0  (1‑based index) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1, but harmless */

    if (cache_size < n)
    {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (new_cache == NULL)
            abort();   /* allocation failed */
        cache = new_cache;

        for (idx_t i = cache_size + 1; i <= n; ++i)
        {
            seq_t val;
            if (i == 1)
                val = (seq_t)1;
            else if (i == 2)
                val = (seq_t)2;
            else if (i == 3)
                val = (seq_t)3;
            else if (i == 4)
                val = (seq_t)6;
            else
            {
                seq_t a1 = cache[i - 2];   /* a(i-1) */
                seq_t a2 = cache[i - 3];   /* a(i-2) */
                seq_t a3 = cache[i - 4];   /* a(i-3) */
                seq_t a4 = cache[i - 5];   /* a(i-4) */

                seq_t term1 = (seq_t)(i - 1) * a4;
                seq_t term2 = (seq_t)2 * (seq_t)i * a3;
                seq_t term3 = (seq_t)(i + 1) * a2;
                seq_t term4 = (seq_t)3 * a1;

                val = (term1 + term2 + term3 - term4) / 2;
            }
            cache[i - 1] = val;
        }
        cache_size = n;
    }

    return cache[n - 1];
}
