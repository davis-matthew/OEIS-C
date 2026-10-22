/*
 * A308494  Refactorable numbers k such that the number of divisors of k-1
 *          also divides k.
 * https://oeis.org/A308494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   build_spf, divisor_count_using_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A308494: list of refactorable integers k such that the
 * number of divisors of k‑1 also divides k.
 * The function builds the sequence on demand, using a growing sieve
 * of smallest prime factors to compute divisor counts efficiently. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_spf.h"          /* for build_spf() */
#include "../utils/divisor_count_using_spf.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for indices < 1 */
        return 0;

    /* static state: cache of generated terms, spf table, etc. */
    static int           initialized = 0;
    static seq_t        *cache      = NULL;
    static size_t        cache_cap  = 0;
    static size_t        cache_len  = 0;
    static idx_t        *spf        = NULL;
    static idx_t        spf_limit  = 0;
    static idx_t         next_k     = 8;   /* first candidate ≥ 8 */

    if (!initialized)
    {
        initialized = 1;
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;

        spf_limit = 2000000;          /* initial sieve limit */
        build_spf(spf_limit, &spf);
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        /* extend sieve if next_k is beyond current limit */
        if (next_k > spf_limit)
        {
            idx_t new_limit = spf_limit * 2;
            if (new_limit < next_k)   /* safety check */
                new_limit = next_k;
            idx_t *new_spf = NULL;
            build_spf(new_limit, &new_spf);
            free(spf);
            spf = new_spf;
            spf_limit = new_limit;
        }

        /* check k: k must be divisible by d(k) */
        idx_t dk = divisor_count_using_spf(next_k, spf);
        if (next_k % dk != 0)
        {
            ++next_k;
            continue;
        }

        /* check k-1: d(k-1) must divide k */
        idx_t dk1 = divisor_count_using_spf(next_k-1, spf);
        if (next_k % dk1 != 0)
        {
            ++next_k;
            continue;
        }

        /* store found term */
        if (cache_len >= cache_cap)
        {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len] = (seq_t)next_k;
        ++cache_len;
        ++next_k;
    }

    return cache[n-1];
}
