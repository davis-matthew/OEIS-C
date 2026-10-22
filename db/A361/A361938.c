/*
 * A361938  a(0)=1, a(1)=0; a(n) = floor(n/2)*(a(n-1) + a(n-2)).
 * https://oeis.org/A361938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0; a(n)=⌊n/2⌋·(a(n-1)+a(n-2))
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute a(n) for OEIS sequence A361938.
   The result is cached, so repeated calls (in any order) are fast.
   The cache grows geometrically as needed. */
seq_t a(idx_t n)
{
    /* persistent data */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;          /* allocated capacity */
    static bool   init  = false;      /* have base terms been set? */
    static idx_t  last  = 1;          /* index of last computed term */

    /* initialise cache and base terms on first use */
    if (!init) {
        /* at least two slots for a(0) and a(1) */
        idx_t newcap = 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        init   = true;
        last   = 1;
    }

    /* Extend the cache and compute as far as required */
    if (n > last) {
        /* grow allocation to at least n+1 slots */
        idx_t newcap = cap;
        while (newcap <= n)
            newcap <<= 1;
        if (newcap != cap) {
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap   = newcap;
        }

        /* compute terms from last+1 up to n */
        for (idx_t k = last + 1; k <= n; ++k) {
            seq_t sum = cache[k - 1] + cache[k - 2];
            seq_t f   = (seq_t)(k >> 1);   /* floor(k/2) */
            cache[k]  = f * sum;
        }
        last = n;
    }

    return cache[n];
}
