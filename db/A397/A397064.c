/*
 * A397064  G.f. A(x) satisfies (1-2*x)*A(x) + A(x^3) = x.
 * https://oeis.org/A397064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n ≥ 2, a(n) = 2 * a(n-1) – (n % 3 == 0 ? a(n/3) :
 *            0)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] stores a(1) */
static size_t cache_len = 0;  /* number of terms stored (≥0) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined by specification */

    /* Initialise cache with a(1) if needed */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;        /* a(1) = 1 */
        cache_len = 1;
    }

    /* Extend the cache up to a(n) if we haven't computed it yet */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;

        for (idx_t j = (idx_t)cache_len + 1; j <= n; ++j) {
            size_t idx = (size_t)(j - 1);        /* index of a(j) in cache */
            seq_t val = cache[idx - 1] << 1;     /* 2 * a(j-1) */
            if (j % 3 == 0)                     /* subtract a(j/3) if divisible by 3 */
                val -= cache[(j / 3) - 1];
            cache[idx] = val;
        }
        cache_len = new_len;
    }

    return cache[(size_t)(n - 1)];
}
