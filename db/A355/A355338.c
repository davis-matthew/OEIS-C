/*
 * A355338  Expansion of e.g.f.: exp(exp(x) - x^2 - 1).
 * https://oeis.org/A355338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -2*(n-1)*a(n-2) + Sum_{k=1..n} binomial(n-1,k-1)*a(n-k)
 *            a(0) = 1, a(1) = 1 Recurrence with memoization; works for any
 *            seq_t and idx_t types
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 18, i64 n <= 28, i128 n <= 45,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;
static unsigned char *ready = NULL;
static size_t cache_cap = 0;

/* Resize the cache to hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* If already big enough, nothing to do. */
    if (cache_cap > (size_t)n) return;

    size_t new_cap = 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

#ifdef __cplusplus
    /* C++: use new/delete so constructors/destructors run. */
    seq_t *new_cache = new seq_t[new_cap];
    unsigned char *new_ready = new unsigned char[new_cap];
    if (cache) {
        /* Preserve already computed values. */
        for (size_t i = 0; i < cache_cap; ++i) {
            new_cache[i] = cache[i];
            new_ready[i] = ready[i];
        }
        delete[] cache;
        delete[] ready;
    }
    cache  = new_cache;
    ready  = new_ready;
#else
    /* C: realloc works for trivial types. */
    cache  = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    ready  = (unsigned char*)realloc(ready, new_cap * sizeof(unsigned char));
#endif

    /* New entries are uninitialized; mark them as not ready. */
    for (size_t i = cache_cap; i < new_cap; ++i) ready[i] = 0;
    cache_cap = new_cap;
}

/* Main sequence routine. */
seq_t
a(idx_t n)
{
    if (n == (idx_t)0 || n == (idx_t)1) return (seq_t)1;

    ensure_cache(n);

    for (idx_t i = (idx_t)0; i <= n; ++i) {
        if (ready[i]) continue;          /* already computed */

        if (i == (idx_t)0 || i == (idx_t)1) {
            cache[i] = (seq_t)1;
            ready[i] = 1;
            continue;
        }

        /* Sum_{k=1..i} binomial(i-1,k-1)*a(i-k)  */
        seq_t sum = 0;
        for (idx_t j = (idx_t)0; j <= i-1; ++j) {
            seq_t bin = binomial(i-1, j);               /* C(i-1,j) */
            sum += bin * cache[i-1-j];
        }

        /* Term -2*(i-1)*a(i-2) */
        seq_t term = -( (seq_t)2 * (seq_t)(i-1) * cache[i-2] );

        cache[i] = term + sum;
        ready[i] = 1;
    }

    return cache[n];
}
