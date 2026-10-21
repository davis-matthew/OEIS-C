/*
 * A255020  Number of (n+2) X (1+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 0 or 1 and no antidiagonal sum 0 or 1 and no row sum
 *          0 or 1 and no column sum 0 or 1.
 * https://oeis.org/A255020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) + 4*a(n-4) + 4*a(n-5) + 2*a(n-6) for
 *            n>7 Initial values: a(1)=26, a(2)=54, a(3)=117, a(4)=269,
 *            a(5)=601, a(6)=1306, a(7)=2853
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 25, u64 n <= 53, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_CACHE 10000   /* number of terms we cache to avoid repeated work */

static seq_t cache[MAX_CACHE + 1];  /* index 0 unused */
static idx_t cache_cap = 0;          /* maximum index computed in cache */

/* Ensure that cache has values up to idx n (if n <= MAX_CACHE) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;

    /* Initialize the first seven terms if not yet done. */
    if (cache_cap < 7) {
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)54;
        cache[3] = (seq_t)117;
        cache[4] = (seq_t)269;
        cache[5] = (seq_t)601;
        cache[6] = (seq_t)1306;
        cache[7] = (seq_t)2853;
        cache_cap = 7;
    }

    /* Compute from the first missing index up to n, but not beyond MAX_CACHE. */
    idx_t start = cache_cap + 1;
    for (idx_t i = start; i <= n && i <= MAX_CACHE; ++i) {
        cache[i] = cache[i-1]
                + (seq_t)3 * cache[i-3]
                + (seq_t)4 * cache[i-4]
                + (seq_t)4 * cache[i-5]
                + (seq_t)2 * cache[i-6];
    }

    if (n <= MAX_CACHE)
        cache_cap = n;
    else
        cache_cap = MAX_CACHE;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n = 0, return 0 safely */

    if (n <= MAX_CACHE) {
        ensure_cache(n);
        return cache[n];
    }

    /* n larger than cached size: first ensure cached portion is full */
    ensure_cache(MAX_CACHE);

    /* Keep last six values in a circular buffer */
    seq_t last[6];
    for (idx_t j = 0; j < 6; ++j)
        last[j] = cache[MAX_CACHE - j];

    seq_t cur = 0;
    for (idx_t i = MAX_CACHE + 1; i <= n; ++i) {
        cur = last[0]
            + (seq_t)3 * last[2]
            + (seq_t)4 * last[3]
            + (seq_t)4 * last[4]
            + (seq_t)2 * last[5];

        /* shift right, insert new cur at position 0 */
        for (int j = 5; j > 0; --j)
            last[j] = last[j-1];
        last[0] = cur;
    }
    return cur;
}
