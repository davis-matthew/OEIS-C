/*
 * A309678  G.f. A(x) satisfies: A(x) = A(x^4) / (1 - x)^2.
 * https://oeis.org/A309678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊n/4⌋} (n - 4k + 1) * a(k), a(0) = 1
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation stores a(0)…a(n) in a growing static cache. */

#include <stdlib.h>
#include <string.h>

/* Static cache for already computed terms */
static seq_t *cache   = NULL;   /* array of computed values */
static size_t cache_sz  = 0;    /* allocated size (elements) */
static idx_t  cache_hi  = (idx_t)-1;   /* highest index that has been computed */

/* Ensure that the cache can hold at least indices 0…n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < (idx_t)cache_sz)
        return;

    /* Grow capacity to a power of two greater than n+1 */
    size_t new_sz = cache_sz ? cache_sz : 1;
    while (new_sz <= (size_t)n)
        new_sz <<= 1;

    /* Reallocate memory */
    if (cache == NULL) {
        cache = (seq_t *)calloc(new_sz, sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    }

    /* Zero the newly allocated portion */
    if (cache != NULL && new_sz > cache_sz)
        memset(cache + cache_sz, 0, (new_sz - cache_sz) * sizeof(seq_t));

    cache_sz = new_sz;

    /* Initialise a(0) if it has not been set yet */
    if (cache_hi == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache_hi  = 0;
    }
}

/* Compute the n-th term of A309678 (offset 0) */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* If already computed, simply return it */
    if (n <= cache_hi)
        return cache[n];

    /* Compute terms from cache_hi+1 up to n */
    for (idx_t i = cache_hi + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t kmax = i / 4;
        for (idx_t k = 0; k <= kmax; ++k) {
            seq_t coeff = (seq_t)(i - (seq_t)4 * k + 1);
            sum += coeff * cache[k];
        }
        cache[i] = sum;
    }

    cache_hi = n;
    return cache[n];
}
