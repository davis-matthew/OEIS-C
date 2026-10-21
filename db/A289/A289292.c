/*
 * A289292  Coefficients in expansion of E_4^(1/2).
 * https://oeis.org/A289292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 4, i64 n <= 8, i128 n <= 17,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(0) = 1
 *            a(n) = (240 * sigma_3(n) - Σ_{k=1}^{n-1} a(k) * a(n-k)) / 2   (n ≥ 1)
 *
 * Here sigma_3(n) is the sum of the cubes of the divisors of n (via
 * the helper function sigma_k(...,3)).  The recurrence follows from
 * (E4(q))^(1/2)^2 = E4(q).  The implementation memoises previously
 * computed values so that calls with arbitrary n are serviced in
 * O(1) after the needed terms have been filled.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/sigma_k.h"

/* static cache for previously computed terms */
static seq_t *cache = NULL;   /* array of computed terms          */
static size_t cache_size = 0; /* number of valid entries in cache */

static void grow_cache(idx_t upto)
{
    if (cache_size > (size_t)upto)                /* cache already large enough */
        return;

    size_t new_size = (size_t)upto + 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) /* allocation failure: keep existing cache */
        return;

    /* copy existing terms */
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* free old buffer */
    if (cache)
        free(cache);

    cache = new_cache;

    /* compute new terms that were not present */
    for (size_t idx = cache_size; idx < new_size; ++idx)
    {
        if (idx == 0)            /* a(0) = 1 */
        {
            cache[0] = (seq_t)1;
            continue;
        }

        /* 240 * sigma_3(idx) */
        seq_t sigma3 = sigma_k((idx_t)idx, (uint8_t)3);
        seq_t term   = sigma3 * (seq_t)240;

        /* convolution sum Σ_{k=1}^{idx-1} a(k) * a(idx-k) */
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k < idx; ++k)
            sum += cache[k] * cache[idx - k];

        /* a(idx) = (term - sum) / 2 */
        cache[idx] = (term - sum) / (seq_t)2;
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)-1)  /* protect against unsigned wrap-around */
        return (seq_t)0;

    if (n >= (idx_t)cache_size)
        grow_cache(n);

    return cache[(size_t)n];
}
