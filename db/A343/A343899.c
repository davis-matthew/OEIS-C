/*
 * A343899  a(n) = Sum_{k=0..n} (k!)^k * binomial(n,k).
 * https://oeis.org/A343899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k!)^k * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"   /* provides big_pow(seq_t base, seq_t exp) */

/* Cached factorials and (k!)^k values */
static seq_t *fact_cache = NULL;
static seq_t *t_cache   = NULL;
static size_t cache_cap = 0;

/* Ensure the caches contain entries up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t target = (size_t)n + 1;
    if (target <= cache_cap)
        return;

    /* Resize the arrays */
    fact_cache = (seq_t *)realloc(fact_cache, target * sizeof(seq_t));
    t_cache   = (seq_t *)realloc(t_cache,   target * sizeof(seq_t));

    /* Initialize from the current capacity up to n */
    if (cache_cap == 0) {
        /* First entry */
        fact_cache[0] = (seq_t)1;
        t_cache[0]    = (seq_t)1;
        cache_cap = 1;
    }

    for (idx_t k = cache_cap; k <= n; ++k) {
        /* factorial */
        fact_cache[k] = fact_cache[k - 1] * (seq_t)k;

        /* (k!)^k  ->  big_pow takes seq_t for both arguments */
        t_cache[k] = big_pow(fact_cache[k], (seq_t)k);
    }

    cache_cap = target;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    ensure_cache(n);

    seq_t result  = (seq_t)0;
    seq_t binom   = (seq_t)1;  /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = C(n,k) * (k!)^k */
        result += binom * t_cache[k];

        if (k == n)
            break;

        /* Update binomial for next k:
           C(n,k+1) = C(n,k) * (n - k) / (k + 1)   (exact division) */
        binom = binom * (seq_t)(n - k);
        binom = binom / (seq_t)(k + 1);
    }

    return result;
}
