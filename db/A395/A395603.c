/*
 * A395603  Sum of the distinct prime divisors of C(8*n, n).
 * https://oeis.org/A395603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sopf( binomial(8*n , n) ) – sum of distinct prime
 *            divisors of C(8*n,n)
 * Range:     u8 n <= 9, u16 n <= 196, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static idx_t current_limit = 0;          /* maximum 8*n for which primes are currently built */
static uint64_t *primes_cache = NULL;    /* array of primes up to current_limit */
static size_t primes_cache_count = 0;    /* number of primes in primes_cache */

/* Ensure that primes_cache contains all primes <= 8*n */
static void
ensure_primes(idx_t n)
{
    idx_t limit = (idx_t)8 * n;
    if (limit <= current_limit)
        return;
    if (primes_cache != NULL)
        free(primes_cache);
    build_primes_upto(limit, &primes_cache, &primes_cache_count);
    current_limit = limit;
}

seq_t
a(idx_t n)
{
    if (n == 0)     /* the offset is 1; behaviour for n==0 is irrelevant */
        return 0;

    ensure_primes(n);

    idx_t max  = (idx_t)8 * n;   /* 8*n in the binomial coefficient */
    idx_t min  = n;              /* n in the coefficient */
    idx_t sub  = (idx_t)7 * n;   /* 7*n = 8*n - n */

    seq_t sum = 0;

    for (size_t i = 0; i < primes_cache_count; ++i) {
        uint64_t p = primes_cache[i];
        /* All primes in primes_cache satisfy p <= max */
        bool present = false;
        idx_t pk = p;
        while (pk <= max) {
            idx_t term = (max / pk) - (min / pk) - (sub / pk);
            if (term > 0) {
                present = true;
                break;
            }
            /* avoid overflow of pk *= p */
            if (pk > max / p)
                break;
            pk *= p;
        }
        if (present)
            sum += (seq_t)p;
    }

    return sum;
}
