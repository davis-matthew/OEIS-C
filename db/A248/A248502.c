/*
 * A248502  Numbers m that are not coprime to floor(m/16).
 * https://oeis.org/A248502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th positive integer m such that gcd(m,
 *            floor(m/16)) > 1
 * Range:     u8 n <= 110, u16 n <= 8874, u32 n <= 8874, u64 n <= 8874,
 *            u128 n <= 8874, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Static cache that grows as needed.  Cached values are stored 0‑based. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_m = 1;          /* last integer already checked */

    /* Expand the cache until we have at least n terms. */
    while (cache_len < (size_t)n)
    {
        seq_t m = last_m + 1;
        seq_t t = m / (seq_t)16u;        /* floor(m/16) */
        if (gcd(m, t) > (seq_t)1u)
        {
            /* Grow capacity if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }
        last_m = m;
    }

    return cache[(size_t)n - 1];
}
