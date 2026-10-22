/*
 * A354733  a(0) = a(1) = 1; a(n) = 2 * Sum_{k=0..n-2} a(k) * a(n-k-2).
 * https://oeis.org/A354733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354733
   a(0) = a(1) = 1
   a(n) = 2 * Σ_{k=0}^{n-2} a(k) * a(n-2-k)
   (for n ≥ 2)                                                    */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* cached sequence values      */
static size_t cache_cap = 0;         /* allocated capacity         */
static size_t cache_size = 0;        /* number of terms stored     */

/* Grow the cache so that it contains at least (n+1) terms. */
static void __ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    /* Grow capacity exponentially until it covers n. */
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* allocation failure */

    cache = new_cache;
    cache_cap = new_cap;

    /* Compute missing terms. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i == 0 || i == 1)
            cache[i] = (seq_t)1;
        else {
            seq_t sum = (seq_t)0;
            idx_t m = (idx_t)i;
            /* Compute 2 * Σ a[k] * a[m-2-k] for k = 0..m-2 */
            for (idx_t k = 0; k <= m - 2; ++k)
                sum = sum + cache[k] * cache[(size_t)(m - 2 - k)];
            cache[i] = sum * 2;
        }
    }
    cache_size = (size_t)n + 1;
}

/* Sequence function required by the build. */
seq_t a(idx_t n)
{
    __ensure_cache(n);
    return cache[(size_t)n];
}
