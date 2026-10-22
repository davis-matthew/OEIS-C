/*
 * A343523  a(0) = 1; a(n) = 2 * Sum_{k=1..n} binomial(n,k) * a(k-1).
 * https://oeis.org/A343523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1;  a(n) = 2 * Σ{k=1..n} binomial(n,k) * a(k-1)    (n ≥ 1)
 */
#include <stdlib.h>
#include <stddef.h>

/* Two-phase static cache: cache[i] holds a(i) for i < cache_size. */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Return a(n).  Computes all values up to n on demand and caches the result. */
seq_t a(idx_t n)
{
    /* Initialise base case when first called. */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* OOM – unlikely in tests */
        cache[0] = (seq_t)1;                  /* a(0) */
        cache_size = 1;
    }

    /* If we already have the requested term, return it. */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Grow cache to hold terms up to n. */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;              /* OOM – unlikely in tests */
    cache = tmp;
    /* All newly allocated entries are zero-initialised. */

    /* Compute missing terms, from cache_size up to n inclusive. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i == 0) {                       /* a(0) already set */
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        seq_t binom = 1;                     /* C(i,0) */
        for (size_t k = 1; k <= i; ++k) {
            /* binom = binom * (i - k + 1) / k   using seq_t arithmetic */
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
            sum += binom * cache[k - 1];
        }
        cache[i] = sum * (seq_t)2;
    }

    cache_size = new_size;
    return cache[(size_t)n];
}
