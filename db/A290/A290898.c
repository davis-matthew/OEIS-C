/*
 * A290898  p-INVERT of the positive integers, where p(S) = 1 - S - S^4.
 * https://oeis.org/A290898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 34*a(n-2) + 71*a(n-3) - 89*a(n-4) + 71*a(n-5)
 *            - 34*a(n-6) + 9*a(n-7) - a(n-8), initial values: a(0)=1, a(1)=3,
 *            a(2)=8, a(3)=22, a(4)=65, a(5)=203, a(6)=647, a(7)=2053
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static size_t   cache_cap = 0;   /* capacity of cache array */
static size_t   cache_len = 0;   /* number of terms currently stored */

/* initial 8 terms */
static const seq_t init[8] = {1, 3, 8, 22, 65, 203, 647, 2053};

seq_t a(idx_t n)
{
    /* ensure cache is initialized */
    if (cache == NULL) {
        cache_cap = 16;                      /* start with a small capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure → 0 (unlikely) */

        /* copy initial terms into cache */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        cache_len = 8;
    }

    /* if n is already computed, return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* grow cache if necessary */
    size_t needed_cap = n + 1;
    if (needed_cap > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms from cache_len up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t t1 = 9 * cache[i-1];
        seq_t t2 = 34 * cache[i-2];
        seq_t t3 = 71 * cache[i-3];
        seq_t t4 = 89 * cache[i-4];
        seq_t t5 = 71 * cache[i-5];
        seq_t t6 = 34 * cache[i-6];
        seq_t t7 = 9 * cache[i-7];
        seq_t t8 = cache[i-8];

        /* Compute as (positive terms) - (negative terms)
           The recurrence guarantees the result is non‑negative. */
        cache[i] = t1 + t3 + t5 + t7 - t2 - t4 - t6 - t8;
    }
    cache_len = n + 1;

    return cache[(size_t)n];
}
