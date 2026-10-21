/*
 * A261304  a(n+1) = abs(a(n) - gcd(a(n), 4n+3)), a(1) = 1.
 * https://oeis.org/A261304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   a(n+1) = abs(a(n) - gcd(a(n), 4n+3))   (OEIS A261304) */
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache with 1‑based indexing: cache[0] holds a(1) */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;  /* number of terms stored */

    if (n == 0) /* undefined for n < 1, but never called */
        return (seq_t)0;

    /* Grow cache if necessary */
    if (n > cached_len)
    {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache)
            abort();           /* allocation failure */

        /* If no cache yet, initialise the first term */
        if (cached_len == 0)
        {
            new_cache[0] = (seq_t)1;  /* a(1) */
            cached_len = 1;
        }

        /* Compute subsequent terms */
        seq_t a_prev = new_cache[cached_len - 1];
        for (idx_t i = cached_len + 1; i <= n; ++i)
        {
            seq_t k = (seq_t)(i - 1);               /* current n in recurrence */
            seq_t g = gcd(a_prev, (seq_t)4 * k + (seq_t)3);
            seq_t next = (a_prev > g) ? (a_prev - g) : (g - a_prev);
            new_cache[i - 1] = next;
            a_prev = next;
        }

        cache = new_cache;
        cached_len = n;
    }

    return cache[n - 1];
}
