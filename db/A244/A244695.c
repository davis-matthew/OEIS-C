/*
 * A244695  Number of length n+4 0..3 arrays with no five consecutive elements
 *          with pattern ababa or abbba (with a!=b) and new values 0..3
 *          introduced in 0..3 order.
 * https://oeis.org/A244695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) - 5*a(n-3) + a(n-4) - 3*a(n-5) * +
 *            4*a(n-6) - 4*a(n-7) + a(n-8) + 3*a(n-9) + 2*a(n-11) * - a(n-12)
 *            + a(n-13) * Initial values: a(1)=49, a(2)=176, a(3)=661,
 *            a(4)=2540, a(5)=9858, * a(6)=38440, a(7)=150208, a(8)=587527,
 *            a(9)=2299087, * a(10)=8998528, a(11)=35223120, a(12)...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[14] = {
    0ULL,                /* index 0 unused */
    49ULL, 176ULL, 661ULL, 2540ULL, 9858ULL,
    38440ULL, 150208ULL, 587527ULL, 2299087ULL,
    8998528ULL, 35223120ULL, 137880423ULL, 539741489ULL
};

static seq_t *cache = NULL;        /* 1‑based cache of a(n) */
static idx_t cache_len = 0;        /* highest n currently stored */

/* Return a(n) for n >= 1.  Undefined behaviour for n < 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* out of range; caller will not use this case */

    /* ensure cache holds at least the first 13 values */
    if (cache_len < 13) {
        size_t newlen = 13;
        cache = (seq_t *)realloc(cache, (newlen + 1) * sizeof(seq_t));
        if (cache == NULL)
            return 0;   /* allocation failed */
        for (size_t i = 1; i <= newlen; ++i)
            cache[i] = init_vals[i];
        cache_len = newlen;
    }

    /* Return if already computed */
    if (n <= cache_len)
        return cache[n];

    /* Grow the cache to hold the requested n */
    size_t newlen = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, (newlen + 1) * sizeof(seq_t));
    if (tmp == NULL)
        return 0;   /* allocation failed */
    cache = tmp;

    /* Compute new terms by the empirical recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* sum of positive contributions */
        seq_t pos = 5 * cache[i - 1]
                  + cache[i - 4]
                  + 4 * cache[i - 6]
                  + cache[i - 8]
                  + 3 * cache[i - 9]
                  + 2 * cache[i - 11]
                  + cache[i - 13];

        /* sum of negative contributions */
        seq_t neg = 3 * cache[i - 2]
                  + 5 * cache[i - 3]
                  + 3 * cache[i - 5]
                  + 4 * cache[i - 7]
                  + cache[i - 12];

        cache[i] = pos - neg;   /* recurrence ensures pos >= neg */
    }

    cache_len = n;
    return cache[n];
}
