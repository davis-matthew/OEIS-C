/*
 * A397862  G.f. A(x) satisfies A(x) = 1 + x * (1+x) * A(x * (1+x)^3).
 * https://oeis.org/A397862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0}^{n-1} binomial(3*k+1, n-k-1) * a(k), a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;          /* memoization array for a(n) */
static size_t cache_size = 0;        /* allocated size of cache */
static idx_t max_computed = 0;       /* highest n for which a(n) is stored */

/* Ensure that cache has entries up to n (inclusive).  New elements are zeroed. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cache_size)
        return;

    size_t new_cap = cache_size ? cache_size : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* on allocation failure, terminate with an error code */
        abort();
    }
    cache = tmp;
    for (size_t i = cache_size; i < new_cap; ++i)
        cache[i] = 0;
    cache_size = new_cap;
}

/* Public routine */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (!cache) {
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;            /* a(0) = 1 */
        max_computed = 0;
    }

    if (n <= max_computed)
        return cache[n];

    ensure_capacity(n);

    /* Compute terms from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            /* binomial(3*k+1, i-k-1) * a(k) */
            seq_t c = binomial(3 * k + 1, i - k - 1);
            sum += c * cache[k];
        }
        cache[i] = sum;
    }

    max_computed = n;
    return cache[n];
}
