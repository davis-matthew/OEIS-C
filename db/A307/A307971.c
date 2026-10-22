/*
 * A307971  G.f. A(x) satisfies: A(x) = 1 + x + x^2 + x^3 + x^4*A(x)^2.
 * https://oeis.org/A307971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = a(2) = a(3) = a(4) = 1; for n >= 5, a(n) =
 *            sum_{k=0}^{\,n-4} a(k) * a((n-4)-k) (convolution). This
 *            implements the recurrence a(n+4) = Σ_{k=0..n} a(k)*a(n-k). The
 *            implementation memoizes the values and computes them on demand
 *            using a simple O(n^2) routine, which is fast enough for n...
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 50, u64 n <= 93, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoization */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of filled entries */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure that cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_len > n)
        return;

    /* Resize if necessary */
    size_t needed = n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_buf) abort();
        cache = new_buf;
        cache_cap = new_cap;
    }

    /* Initialise the first five terms if not already done */
    if (cache_len < 5) {
        for (size_t i = cache_len; i < 5 && i < needed; ++i)
            cache[i] = 1;
        cache_len = 5 > cache_len ? 5 : cache_len;
    }

    /* Compute entries from current length up to n */
    for (idx_t m = cache_len; m <= n; ++m) {
        if (m < 5) {
            cache[m] = 1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t k = 0; k <= m - 4; ++k)
            sum += cache[k] * cache[(m - 4) - k];
        cache[m] = sum;
    }
    cache_len = n + 1;
}

/* Public function required by the build harness */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
