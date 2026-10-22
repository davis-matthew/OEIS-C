/*
 * A336635  Sum_{n>=0} a(n) * x^n / (n!)^2 = exp(BesselI(0,2*sqrt(x))^2 - 1).
 * https://oeis.org/A336635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = (1/n) * Σ_{k=1..n} binomial(n,k)^2 *
 *            binomial(2*k,k) * k * a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Static cache for memoised values */
static seq_t *cache = NULL;
static size_t cache_size = 0;      /* allocated length of cache */
static size_t computed_up_to = 0;  /* highest index that has been computed */

/* Ensure the cache holds values up to target_n (inclusive) */
static void ensure_up_to(idx_t target_n)
{
    size_t t = (size_t)target_n;
    /* Grow the cache if necessary */
    if (t + 1 > cache_size) {
        size_t new_size = t + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache)           /* allocation failure – unlikely */
            return;
        /* Copy existing values if any */
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        /* Zero the new slots */
        memset(new_cache + cache_size, 0,
               (new_size - cache_size) * sizeof(seq_t));
        cache = new_cache;
        cache_size = new_size;
        /* Initialise a(0) after first allocation (or whenever it may be zero) */
        cache[0] = (seq_t)1;
    }

    /* Compute any missing values */
    size_t start = 1;
    if (computed_up_to >= 1)
        start = computed_up_to + 1;

    for (size_t i = start; i <= t; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= (idx_t)i; ++k) {
            seq_t bnk  = binomial((idx_t)i, k);          /* binomial(i,k)   */
            seq_t term = bnk;
            term      *= bnk;                            /* square it       */
            term      *= binomial(2 * k, k);             /* * binomial(2k,k) */
            term      *= (seq_t)k;                       /* * k             */
            term      *= cache[i - (size_t)k];           /* * a(n-k)        */
            sum       += term;
        }
        cache[i] = sum / (seq_t)i;                       /* exact division  */
    }
    computed_up_to = t;
}

seq_t a(idx_t n)
{
    if ((idx_t)0 > n) return (seq_t)0;  /* Guard for negative indices, though idx_t is unsigned */
    ensure_up_to(n);
    return cache[(size_t)n];
}
