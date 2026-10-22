/*
 * A351344  G.f. A(x) satisfies: A(x) = 1 + x + x^2 + x^3 + x^4 + x^5 * A(x/(1
 *          - 2*x)) / (1 - 2*x).
 * https://oeis.org/A351344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A351344
   a(0..4) = 1
   a(n) = Σ_{k=0}^{n-5} binomial(n-5,k) * 2^k * a(n-5-k)   for n ≥ 5
   The code memoises values; it works for calls in any order. */

#include <stdlib.h>

static seq_t *cache     = NULL;   /* array of cached values */
static size_t cache_cap = 0;      /* current allocated capacity */
static size_t cache_sz  = 0;      /* number of values that have been computed */

static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                   /* out of memory – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise cache with the first five terms if not yet done. */
    if (cache_sz == 0) {
        ensure_cache_capacity(16);
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = (seq_t)1;
        cache_sz = 5;
    }

    /* If the requested term is already cached, return it. */
    if ((size_t)n < cache_sz)
        return cache[(size_t)n];

    /* Make sure we have space for n + 1 entries. */
    ensure_cache_capacity((size_t)n + 1);

    /* Compute terms from cache_sz up to n. */
    for (idx_t m = cache_sz; m <= n; ++m) {
        if (m < 5) {
            /* The first five values are already 1. */
            continue;
        }
        idx_t offset = m - 5;                /* m = offset + 5 */
        seq_t sum   = 0;
        seq_t binom = 1;                     /* C(offset,0) */
        seq_t pow2  = 1;                     /* 2^0 */

        for (idx_t k = 0; k <= offset; ++k) {
            /* term = binom * pow2 * a(offset - k) */
            seq_t term = binom;
            term = term * pow2;
            term = term * cache[(size_t)(offset - k)];
            sum   = sum + term;

            /* Prepare next binomial coefficient: C(offset, k+1) */
            if (k < offset) {
                binom = binom * (seq_t)(offset - k);
                binom = binom / (seq_t)(k + 1);
            }

            /* Next power of two */
            pow2 = pow2 * (seq_t)2;
        }

        cache[(size_t)m] = sum;
    }

    cache_sz = (size_t)n + 1;
    return cache[(size_t)n];
}
