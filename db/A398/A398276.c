/*
 * A398276  Number of total dominating sets in the 3 X n king graph.
 * https://oeis.org/A398276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=47, a(3)=353, a(4)=2545, a(5)=19651,
 *            a(6)=150719, a(7)=1149593; For n ≥ 8: a(n) = 6*a(n-1) + 7*a(n-2)
 *            + 40*a(n-3) + 20*a(n-4) - 24*a(n-5) - 5*a(n-6) + 10*a(n-7) -
 *            5*a(n-8) (Linear recurrence of order 8 derived from the rational
 *            generating function.)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initial values */
    static const seq_t init[8] = {
        1,               /* a(0) */
        3,               /* a(1) */
        47,              /* a(2) */
        353,             /* a(3) */
        2545,            /* a(4) */
        19651,           /* a(5) */
        150719,          /* a(6) */
        1149593          /* a(7) */
    };

    /* allocate and populate cache on first call */
    if (!cache) {
        cache_size = 8;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
    }

    /* quick return for small indices */
    if (n < cache_size)
        return cache[(size_t)n];

    /* extend cache up to index n */
    size_t target = (size_t)n;
    cache = (seq_t *)realloc(cache, (target + 1) * sizeof(seq_t));
    if (!cache) return (seq_t)0;  /* allocation failure */

    /* compute missing terms */
    for (size_t i = cache_size; i <= target; ++i) {
        seq_t pos = 0, neg = 0;
        /* positive contributions */
        pos += mul_small(cache[i - 1], 6);
        pos += mul_small(cache[i - 2], 7);
        pos += mul_small(cache[i - 3], 40);
        pos += mul_small(cache[i - 4], 20);
        pos += mul_small(cache[i - 7], 10);
        /* negative contributions */
        neg += mul_small(cache[i - 5], 24);
        neg += mul_small(cache[i - 6], 5);
        neg += mul_small(cache[i - 8], 5);
        /* final value (pos ≥ neg for n ≥ 8) */
        cache[i] = pos - neg;
    }

    cache_size = target + 1;
    return cache[(size_t)n];
}
