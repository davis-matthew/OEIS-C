/*
 * A281662  (Denominator of Bernoulli(2*n)) read mod n.
 * https://oeis.org/A281662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002445(n) (mod n)
 * Range:     u8 n <= 279, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A002445(n) is the denominator of the Bernoulli number B_{2n}: */
/*          A002445(n) = ∏_{p prime, p-1 | 2n} p         */

/* Include utilities for prime generation */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Compute the sequence value */
seq_t a(idx_t n)
{
    /* handle the special case n = 0 (unused, but safe) */
    if (n == 0)
        return (seq_t)0;

    /* static cache of primes */
    static uint64_t *primes = NULL;      /* array of primes */
    static size_t   primes_cnt = 0;      /* number of primes stored */
    static idx_t    primes_limit = 0;    /* highest prime computed (≈ limit) */

    /* limit needed for this n: primes up to 2*n+1 are sufficient */
    idx_t required_limit = 2 * n + 1;
    if (required_limit > primes_limit) {
        /* free previous list if any */
        if (primes != NULL)
            free(primes);
        /* build new prime list */
        build_primes_upto(required_limit, &primes, &primes_cnt);
        primes_limit = required_limit;
    }

    /* compute the product modulo n */
    uint64_t two_n = 2 * n;
    uint64_t res = 1 % (uint64_t)n;   /* initialise to 1 mod n */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (two_n % (p - 1) == 0) {
            uint64_t p_mod = p % (uint64_t)n;
            res = (res * p_mod) % (uint64_t)n;
        }
    }

    return (seq_t)res;
}
