/*
 * A253220  Number of n X 5 nonnegative integer arrays with upper left 0 and
 *          lower right its king-move distance away minus 2 and every value
 *          within 2 of its king move distance from the upper left and every
 *          value increasing by 0 or 1 with every step right, diagonally se or
 *          down.
 * https://oeis.org/A253220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n−1) − 3*a(n−2) + a(n−3) for n > 11 with base values
 *            a(1) = 6 , a(2) = 25 , a(3) = 102 , a(4) = 268 , a(5) = 268 ,
 *            a(6) = 3568 , a(7) = 16028 , a(8) = 40238 , a(9) = 77063 , a(10)
 *            = 126673 , a(11) = 189083
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Pre‑computed values for n = 1 … 11.  Index 0 is unused. */
static const seq_t base[12] = {
    0ULL,            /* 0 (unused) */
    6ULL,            /* a(1) */
    25ULL,           /* a(2) */
    102ULL,          /* a(3) */
    268ULL,          /* a(4) */
    268ULL,          /* a(5) */
    3568ULL,         /* a(6) */
    16028ULL,        /* a(7) */
    40238ULL,        /* a(8) */
    77063ULL,        /* a(9) */
    126673ULL,       /* a(10) */
    189083ULL        /* a(11) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n <= 11) return base[n];

    /* Static cache of previously computed values. */
    static seq_t *cache = NULL;
    static size_t cached = 0;

    /* First call: allocate cache and fill base values. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 12);
        for (size_t i = 1; i <= 11; ++i)
            cache[i] = base[i];
        cached = 11;
    }

    /* If requested value is already cached, return it. */
    if ((size_t)n <= cached)
        return cache[n];

    /* Grow cache to hold up to n. */
    size_t newcap = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
    /* Assume allocation succeeds – the build environment guarantees enough memory. */

    /* Compute values using the recurrence. */
    for (size_t i = cached + 1; i <= n; ++i) {
        /* a(i) = 3*a(i-1) - 3*a(i-2) + a(i-3) */
        seq_t t1 = 3 * cache[i - 1];
        seq_t t2 = 3 * cache[i - 2];
        cache[i] = t1 - t2 + cache[i - 3];
    }

    cached = (size_t)n;
    return cache[n];
}
