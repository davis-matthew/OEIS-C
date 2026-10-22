/*
 * A386448  G.f. A(x) satisfies A(x) = 1/(1 - x - x^3*A''(x)).
 * https://oeis.org/A386448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n-1) + Σ_{k=0}^{n-1} (-k + k²) * a(k) *
 *            a(n-1-k) for n ≥ 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Persistent cache for computed terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored (next free index) */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure the cache can hold values up to index n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)            /* allocation failed: abort */
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n), memoising results. */
seq_t
a(idx_t n)
{
    /* Handle the base case immediately. */
    if (n == 0) {
        if (cache_len == 0) {
            ensure_cache(0);
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        return cache[0];
    }

    /* Initialise cache[0] if necessary. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Grow cache if needed. */
    ensure_cache(n);

    /* Compute any missing terms up to n. */
    for (idx_t m = cache_len; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        /* Σ_{k=0}^{m-1} (k² - k) * a(k) * a(m-1-k).  Skip k=0,1 as weight is zero. */
        for (idx_t k = 2; k < m; ++k) {
            seq_t k_seq = (seq_t)k;
            seq_t weight = k_seq * (k_seq - (seq_t)1);        /* k² - k */
            seq_t prod   = cache[k] * cache[m - 1 - k];
            sum += weight * prod;
        }
        cache[m] = cache[m - 1] + sum;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
