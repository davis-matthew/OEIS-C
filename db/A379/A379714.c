/*
 * A379714  Partial alternating sums of the number of exponential divisors
 *          function (A049419).
 * https://oeis.org/A379714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ∑_{k=1..n} (-1)^(k+1) A049419(k)
 * Range:     i8 n <= 391, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A049419(k) = ∏_{p^e‖k} τ(e)  */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/divisor_count_u64.h"

/* Static cache for the exponential divisor counts (exp_cache)
   and the alternating sums (alt_cache).  */
static seq_t *exp_cache = NULL;          /* a(k) = A049419(k)            */
static seq_t *alt_cache = NULL;          /* cumulative alternating sum  */
static size_t cache_cap = 0;             /* allocated array size         */
static idx_t  last_computed = 0;         /* highest k already computed   */

/* Smallest prime factor table.  */
static uint64_t *spf = NULL;
static size_t spf_cap = 0;                /* allocated spf size           */

/* Ensure spf is built up to n (inclusive).  */
static void ensure_spf(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (spf_cap >= need) return;

    size_t new_cap = need;
    uint64_t *new_spf = (uint64_t*)calloc(new_cap, sizeof(uint64_t));
    if (!new_spf) abort();

    for (size_t i = 2; i < new_cap; ++i)
    {
        if (new_spf[i] == 0)
        {
            new_spf[i] = (uint64_t)i;
            if (i * i < new_cap)
            {
                for (size_t j = i * i; j < new_cap; j += i)
                    if (new_spf[j] == 0) new_spf[j] = (uint64_t)i;
            }
        }
    }

    free(spf);
    spf     = new_spf;
    spf_cap = new_cap;
}

/* Ensure the cache arrays can hold indices up to n (inclusive).  */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache_cap >= need) return;

    /* Grow capacity (at least double, but not less than need). */
    size_t new_cap = cache_cap * 2 + 1;
    if (new_cap < need) new_cap = need;

    seq_t *new_exp = (seq_t*)malloc(new_cap * sizeof(seq_t));
    seq_t *new_alt = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_exp || !new_alt) abort();

    if (exp_cache)
    {
        memcpy(new_exp, exp_cache, cache_cap * sizeof(seq_t));
        memcpy(new_alt, alt_cache, cache_cap * sizeof(seq_t));
        free(exp_cache);
        free(alt_cache);
    }

    exp_cache = new_exp;
    alt_cache = new_alt;
    cache_cap = new_cap;

    /* Initialise the zeroth indices. */
    exp_cache[0] = (seq_t)0;
    alt_cache[0] = (seq_t)0;
}

/* Fill the caches up to index n (inclusive).  */
static void fill_to(idx_t n)
{
    ensure_cache(n);
    ensure_spf(n);

    for (idx_t k = last_computed + 1; k <= n; ++k)
    {
        if (k == 1)
        {
            exp_cache[1] = (seq_t)1;
            alt_cache[1] = (seq_t)1;
            continue;
        }

        uint64_t x = (uint64_t)k;
        seq_t prod = (seq_t)1;

        /* Factor k using SPF and compute product of τ(e). */
        while (x > 1)
        {
            uint64_t p = spf[x];
            int e = 0;
            while (x % p == 0) { x /= p; ++e; }
            seq_t tau_e = divisor_count_u64((uint64_t)e);
            prod *= tau_e;
        }

        exp_cache[k] = prod;
        seq_t sign = ((k & 1) ? (seq_t)1 : (seq_t)(-1));
        alt_cache[k] = alt_cache[k-1] + sign * prod;
    }

    last_computed = n;
}

/* Public interface.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    fill_to(n);
    return alt_cache[n];
}
