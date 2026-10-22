/*
 * A305889  a(n) = 3*a(n-2) + a(n-4), a(0)=a(1)=0, a(2)=1, a(3)=2.
 * https://oeis.org/A305889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + a(n-4) with a(0)=a(1)=0, a(2)=1, a(3)=2
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 76, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes term a(n) of OEIS sequence A305889. The function keeps a
   static cache so that repeated calls in any order stay linear in the
   maximum requested n.  The cache is built with standard malloc/free,
   which works for any seq_t type. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache data */
    static seq_t *cache   = NULL;   /* pointer to cached values         */
    static size_t cap     = 0;       /* capacity of cache (number of slots) */
    static size_t len     = 0;       /* number of entries already computed */

    /* if value already computed, return it */
    if ((size_t)n < len)
        return cache[n];

    /* enlarge cache if needed */
    if (n + 1 > cap) {
        /* double the capacity until it is large enough */
        size_t newcap = cap ? cap * 2 : 1;
        while (newcap < (size_t)n + 1)
            newcap *= 2;

        /* allocate new storage */
        seq_t *newcache = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!newcache)
            return 0;  /* out‑of‑memory: return zero (should never happen) */

        /* copy old data */
        for (size_t i = 0; i < cap; ++i)
            newcache[i] = cache[i];

        /* free old storage */
        free(cache);

        cache = newcache;
        cap   = newcap;
    }

    /* compute missing values up to n (inclusive) */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i == 0 || i == 1)
            cache[i] = 0;
        else if (i == 2)
            cache[i] = 1;
        else if (i == 3)
            cache[i] = 2;
        else
            cache[i] = cache[i - 2] * (seq_t)3 + cache[i - 4];
    }

    len = (size_t)n + 1;      /* now all indices < len are initialized */
    return cache[(size_t)n];
}
