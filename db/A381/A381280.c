/*
 * A381280  Expansion of e.g.f. 1/(1 - x * cosh(2*x)).
 * https://oeis.org/A381280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Σ_{k=0..⌊(n-1)/2⌋} 4^k * (2k+1) *
 *            binomial(n,2k+1) * a(n-2k-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers for memory allocation */
#include <stdlib.h>
#include <stddef.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;

/* Ensure the cache can hold values up to index n */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if ((size_t)n + 1 > new_cap)
        new_cap = (size_t)n + 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)                 /* Allocation failed – leave cache unchanged */
        return;

    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    ensure_cache_size(n);

    /* Initialise cache[0] the first time */
    if (cache_max == 0) {
        cache[0] = (seq_t)1;
        cache_max = 0;
    }

    /* Compute any required values up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum  = 0;
        seq_t pow4 = 1;         /* 4^k at the current k */
        seq_t comb = (seq_t)i;  /* binomial(i,1) */

        for (idx_t r = 1; r <= i; r += 2) {
            seq_t term = cache[i - r];
            term = term * pow4;
            term = term * comb;
            term = term * (seq_t)r;
            sum += term;

            /* Prepare for the next odd r */
            if (r + 2 <= i) {
                seq_t tmp = comb * (seq_t)(i - r);
                tmp = tmp * (seq_t)(i - r - 1);
                tmp = tmp / (seq_t)(r + 1);
                tmp = tmp / (seq_t)(r + 2);
                comb = tmp;
            }

            if (r + 2 <= i)
                pow4 = pow4 * (seq_t)4;
        }

        cache[i] = sum;
    }

    cache_max = n;
    return cache[n];
}
