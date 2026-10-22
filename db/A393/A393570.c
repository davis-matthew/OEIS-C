/*
 * A393570  Number of ordered set partitions of [n] with palindromic block
 *          sizes.
 * https://oeis.org/A393570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n≥1
   a(n)=1+∑_{s=1}^{⌊n/2⌋} binom(n,s)*binom(n-s,s)*a(n-2s)   */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static caching of already computed terms */
    static seq_t *cache   = NULL;   /* cache[0..max_n] */
    static size_t cache_cap = 0;    /* allocated size of cache */
    static idx_t   max_n      = 0;   /* highest n computed */

    /* first call: allocate space for a(0) */
    if (cache == NULL)
    {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0)=1 */
        max_n   = 0;
    }

    /* expand cache and compute missing values up to n */
    if (n > max_n)
    {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cache_cap)
        {
            /* allocate new array and copy old values */
            seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
            for (size_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        for (idx_t cur = max_n + 1; cur <= n; ++cur)
        {
            /* single block (no pairs) contributes 1 */
            seq_t sum = (seq_t)1;

            idx_t limit = cur / 2;
            seq_t binom1 = (seq_t)1;   /* binom(cur,0) */
            seq_t binom2 = (seq_t)1;   /* binom(cur,0) = 1 as starting point */

            for (idx_t s = 1; s <= limit; ++s)
            {
                /* update binom1 = binom(cur,s) */
                binom1 = binom1 * (seq_t)(cur - s + 1);
                binom1 = binom1 / (seq_t)s;

                /* update binom2 = binom(cur-s,s) using ratio formula
                   f(s) = f(s-1) * ( (cur-2*(s-1))*(cur-2*(s-1)-1) ) / ( s*(cur-(s-1)) ) */
                idx_t sp = s - 1;
                seq_t num = (seq_t)(cur - 2*sp);
                num = num * (seq_t)(cur - 2*sp - 1);
                seq_t den = (seq_t)s * (seq_t)(cur - sp);
                binom2 = binom2 * num;
                binom2 = binom2 / den;

                /* term = a[cur-2s] * binom2 * binom1 */
                seq_t term = cache[cur - 2*s];
                term = term * binom2;
                term = term * binom1;
                sum = sum + term;
            }

            cache[cur] = sum;
        }

        max_n = n;
    }

    return cache[n];
}
