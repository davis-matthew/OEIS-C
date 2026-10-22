/*
 * A380409  Product of prime indices plus sum of prime factors.
 * https://oeis.org/A380409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003963(n) + A001414(n) * A003963(n) : product of prime
 *            indices of n (with multiplicity) * A001414(n) : sum of prime
 *            factors of n (with multiplicity)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Utilities from the framework */
#include "../utils/build_primes_upto.h"   /* build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */
#include "../utils/factor_u64.h"          /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

/* static cache of primes up to the largest processed n */
static uint64_t *primes_cache = NULL;
static size_t primes_cnt_cache = 0;
static idx_t      primes_limit_cache = 0;

/* Helper: binary search for the index (1‑based) of prime p in primes_cache */
static seq_t prime_index(uint64_t p)
{
    size_t low = 0, high = primes_cnt_cache;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (primes_cache[mid] == p)
            return (seq_t)(mid + 1);
        if (primes_cache[mid] < p)
            low = mid + 1;
        else
            high = mid;
    }
    /* Should never happen – p is a prime <= current limit */
    return (seq_t)0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 1, a(0) is unused */
        return (seq_t)1;

    /* Ensure we have all primes up to n cached */
    if (n > primes_limit_cache) {
        if (primes_cache) {
            free(primes_cache);
            primes_cache = NULL;
        }
        build_primes_upto(n, &primes_cache, &primes_cnt_cache);
        primes_limit_cache = n;
    }

    /* Factor n; we need at most 64 distinct primes for any 64‑bit n */
    uint64_t fac[64];
    uint64_t exp[64];
    size_t  fac_cnt = 0;
    factor_u64((uint64_t)n, fac, exp, &fac_cnt);

    seq_t prod = (seq_t)1;
    seq_t sum  = (seq_t)0;

    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = fac[i];
        int e     = (int)exp[i];

        /* Sum of prime factors (with multiplicity) */
        sum += (seq_t)p * (seq_t)e;

        /* Product of prime indices (with multiplicity) */
        seq_t idx = prime_index(p);   /* 1‑based index of prime p */
        for (int k = 0; k < e; ++k)
            prod *= idx;
    }

    return prod + sum;
}
