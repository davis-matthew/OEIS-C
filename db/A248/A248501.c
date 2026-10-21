/*
 * A248501  Numbers m that are coprime to floor(m/16).
 * https://oeis.org/A248501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 145, u16 n <= 8598, u32 n <= 8598, u64 n <= 8598,
 *            u128 n <= 8598, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of A248501: numbers m that are coprime to floor(m/16). */
/* The sequence is built incrementally; a(n) is returned for any n ≥ 1. */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for 0, but safe */

    /* Static cache for previously found terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;    /* number of elements stored */
    static seq_t last_m      = 0;    /* last candidate examined */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n)
    {
        seq_t m = last_m + 1;           /* next candidate value */
        seq_t f = m / (seq_t)16;        /* floor(m/16) */

        if (gcd(m, f) == 1)                 /* keep m if coprime */
        {
            if (cache_len == cache_cap)     /* need more space */
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }
        last_m = m;                       /* advance to next m */
    }

    return cache[n - 1];                   /* a(n) – 1‑based indexing */
}
