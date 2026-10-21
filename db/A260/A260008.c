/*
 * A260008  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000101 00010101 or 01010101.
 * https://oeis.org/A260008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + 7*a(n-4) + a(n-5) + 2*a(n-6) + a(n-7) * -
 *            a(n-8) + a(n-9) (n > 10) * Initial values (a(1) … a(10)): * 36,
 *            49, 109, 209, 436, 828, 1744, 3344, 7005, 13424
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;        /* 1‑based cache storage */
static size_t  cache_cap = 0;      /* allocated capacity  (>= max index + 1) */
static idx_t   max_comp  = 0;      /* highest index that has been computed */

static const seq_t base[11] = {
    0ULL, 36ULL, 49ULL, 109ULL, 209ULL,
    436ULL, 828ULL, 1744ULL, 3344ULL, 7005ULL, 13424ULL
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* out of bounds, should not occur */

    /* Ensure cache has room for indices 0 … n */
    if ((size_t)n + 1 > cache_cap)
        ensure_cache((idx_t)((size_t)n + 1), &cache, &cache_cap);

    /* Initialise base values once */
    if (max_comp < 10) {
        for (idx_t i = 1; i <= 10; ++i)
            cache[(size_t)i] = base[(size_t)i];
        max_comp = 10;
    }

    /* If already computed up to n, return */
    if ((idx_t)n <= max_comp)
        return cache[(size_t)n];

    /* Compute from max_comp+1 up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t val = cache[(size_t)(i - 2)];
        val += val;                               /* 2*a(i-2) */
        val += cache[(size_t)(i - 4)] * (seq_t)7;  /* 7*a(i-4) */
        val += cache[(size_t)(i - 5)];             /* a(i-5)  */
        val += cache[(size_t)(i - 6)] * (seq_t)2;  /* 2*a(i-6)*/
        val += cache[(size_t)(i - 7)];             /* a(i-7)  */
        val -= cache[(size_t)(i - 8)];             /* -a(i-8) */
        val += cache[(size_t)(i - 9)];             /* +a(i-9) */
        cache[(size_t)i] = val;
    }

    max_comp = n;
    return cache[(size_t)n];
}
