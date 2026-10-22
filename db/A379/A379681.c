/*
 * A379681  Sum plus product of the multiset of prime indices of n.
 * https://oeis.org/A379681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A056239(n) + A003963(n)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A056239(n) – sum of the indices of prime factors (counted with multiplicity).  */
/*  A003963(n) – product of the same indices.  */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

/* Binary search – returns 1‑based index of prime p in the sorted array primes[0..cnt-1] */
static size_t prime_index(uint64_t p, uint64_t *primes, size_t cnt)
{
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo + 1;            /* primes[lo] == p is guaranteed for prime factors */
}

/* Cached list of primes up to the largest n requested so far. */
static uint64_t *primes_cache        = NULL;
static size_t    primes_cache_cnt    = 0;
static idx_t     primes_cache_limit  = 0;

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* undefined for n < 1, but harmless */

    /* Ensure we have all primes up to n cached. */
    if ((idx_t)n > primes_cache_limit) {
        free(primes_cache);
        build_primes_upto((idx_t)n, &primes_cache, &primes_cache_cnt);
        primes_cache_limit = n;
    }

    /* Factor n: primes_out holds distinct prime factors,
       exponents holds their multiplicities. */
    uint64_t primes_out[64];
    uint64_t exponents[64];
    size_t   factor_cnt = 0;
    factor_u64((uint64_t)n, primes_out, exponents, &factor_cnt);

    seq_t sum = 0;
    seq_t prod = 1;

    for (size_t i = 0; i < factor_cnt; ++i) {
        size_t  idx  = prime_index(primes_out[i], primes_cache, primes_cache_cnt);
        seq_t   idx_s = (seq_t)idx;

        sum += idx_s * (seq_t)exponents[i];

        /* multiply idx_s exponent times */
        for (size_t k = 0; k < exponents[i]; ++k)
            prod *= idx_s;
    }

    return sum + prod;
}
