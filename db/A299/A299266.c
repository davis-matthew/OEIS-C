/*
 * A299266  Coordination sequence for "cab" 3D uniform tiling formed from
 *          octahedra and truncated cubes.
 * https://oeis.org/A299266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + 2*a(n-3) - 2*a(n-7) + a(n-8) - a(n-9) +
 *            a(n-10) for n >= 13 a(0)=1, a(1)=5, a(2)=9, a(3)=22, a(4)=37,
 *            a(5)=57, a(6)=82, a(7)=117, a(8)=145, a(9)=178, a(10)=229,
 *            a(11)=281, a(12)=322 (values taken from the OEIS entry)
 * Range:     u8 n <= 10, u16 n <= 170, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[13] = {
    1,  /* a(0) */
    5,  /* a(1) */
    9,  /* a(2) */
    22, /* a(3) */
    37, /* a(4) */
    57, /* a(5) */
    82, /* a(6) */
    117,/* a(7) */
    145,/* a(8) */
    178,/* a(9) */
    229,/* a(10) */
    281,/* a(11) */
    322 /* a(12) */
};

seq_t a(idx_t n)
{
    /* static cache to hold computed terms up to the largest n requested */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* length of allocated cache */
    idx_t old_len;
    idx_t i;

    /* initialize cache with the first 13 terms on first call */
    if (cache_len == 0) {
        cache_len = 13;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        for (i = 0; i < 13; i++)
            cache[i] = init_vals[i];
    }

    /* if the requested term is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* grow cache if necessary */
    old_len = cache_len;
    if (n + 1 > cache_len) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        /* reallocation failure should not happen in typical use */
        if (tmp == NULL) abort();
        cache = tmp;
        /* the positions old_len .. 12 are already initialized earlier when the
           cache was first allocated; for safety we reinitialize them again if
           old_len <= 12, but only when the cache length increases beyond 13. */
        if (old_len <= 12) {
            for (i = old_len; i < (n < 12 ? n + 1 : 13); i++)
                cache[i] = init_vals[i];
        }
        cache_len = n + 1;
    }

    /* compute the terms from old_len up to n using the recurrence */
    for (i = old_len; i <= n; i++) {
        seq_t v = cache[i-1] - cache[i-2] + 2*cache[i-3]
                  - 2*cache[i-7] + cache[i-8]
                  - cache[i-9] + cache[i-10];
        cache[i] = v;
    }

    return cache[n];
}
