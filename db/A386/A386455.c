/*
 * A386455  a(0) = 1; a(n) = a(n-1) + Sum_{k=0..n-1} binomial(k+4,5) * a(k) *
 *          a(n-1-k).
 * https://oeis.org/A386455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n-1) + Σ_{k=0..n-1} binomial(k+4,5) * a(k) *
 *            a(n-1-k) (OEIS A386455)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache      = NULL;   /* cache of a(i) values */
static size_t cache_cap  = 0;       /* allocated capacity of cache */
static idx_t  cache_max  = 0;       /* largest i for which cache[i] is valid */

/* Ensure the cache array holds entries up to index n. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 > cache_cap) {
        cache = seq_realloc(cache, cache_cap, (size_t)n + 1);
        cache_cap = (size_t)n + 1;
    }
}

/* Sequence function. */
seq_t a(idx_t n)
{
    /* On first call, allocate the cache and set the initial value. */
    if (cache == NULL) {
        cache_cap = 1;
        cache = seq_realloc(NULL, 0, cache_cap);
        cache[0] = 1;          /* a(0) = 1 */
        cache_max = 0;
    }

    /* Resize cache if necessary. */
    ensure_cache(n);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t b = binomial(k + 4, 5);
            seq_t prod = b * cache[k];
            prod = prod * cache[i - 1 - k];
            sum = sum + prod;
        }
        cache[i] = cache[i - 1] + sum;
    }
    cache_max = n;

    return cache[n];
}
