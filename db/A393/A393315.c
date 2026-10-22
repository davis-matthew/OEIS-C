/*
 * A393315  G.f. A(x) satisfies: A(x) = A(x^3) / (1 - x - x^2).
 * https://oeis.org/A393315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} Fibonacci(n-3*k+1) * a(k) (with
 *            a(0)=1). This implements the recurrence from OEIS A393315
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>     /* for NULL, size_t */
#include <stdlib.h>     /* for realloc, abort */
#include <stdint.h>     /* for uint64_t */
#include "../utils/fib.h"   /* provides fib(idx_t n) -> seq_t */

/* Static cache for memoization */
static seq_t *cache = NULL;          /* array of previously computed values   */
static size_t cap = 0;               /* current capacity of the cache        */
static idx_t  computed_max = (idx_t)-1;  /* largest n already computed */

/* Ensure the cache can hold all indices 0 .. sz-1 */
static void ensure_cache_capacity(idx_t sz)
{
    if (cache == NULL || sz > cap) {
        size_t new_cap = (size_t)sz;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();          /* out of memory */
        cache = new_cache;
        /* Construct any new elements for C++ arbitrary‑precision types. */
#if defined(__cplusplus)
        /* If we had previously computed some values, they are already
           constructed; otherwise we need to initialise from the first
           new element onwards. */
        idx_t start = (computed_max == (idx_t)-1) ? 0
                                                   : computed_max + 1;
        for (size_t i = (size_t)start; i < new_cap; ++i)
            new (&cache[i]) seq_t(); /* default construction */
#endif
        cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Make sure the cache can hold a(n) */
    ensure_cache_capacity(n + 1);

    /* Initialise a(0) if this is the very first call */
    if (computed_max == (idx_t)-1) {
        cache[0] = (seq_t)1;     /* a(0) = 1 */
        computed_max = 0;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t k_max = i / 3;
        for (idx_t k = 0; k <= k_max; ++k) {
            idx_t fib_index = i - 3 * k + 1;
            seq_t f = fib(fib_index);
            sum += f * cache[k];
        }
        cache[i] = sum;
    }
    computed_max = n;

    return cache[n];
}
