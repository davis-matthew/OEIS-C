/*
 * A306261  Least k > 0 such that 2n - p is prime where p is some prime
 *          divisor of 4n^2 - (2k-1)^2 for n >= 4.
 * https://oeis.org/A306261
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A306261:  a(n) = least k > 0 such that there exists a prime divisor p of
    D = 4 n² – (2k–1)² for which 2n – p is prime,  for n ≥ 4.  The code
    searches k increasingly, factors D by trial division with a cached
    prime list, and checks the prime‑difference condition.  All arithmetic
    that can overflow 64 bits is performed in uint64_t; the returned
    value fits in seq_t.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/*  Cached list of primes up to the current limit.  */
static uint64_t *PRIMES = NULL;
static size_t PRIMES_COUNT = 0;
static idx_t  PRIMES_LIMIT = 0;

/*  Ensure that PRIMES contains all primes ≤ limit (inclusive).  */
static void
ensure_primes(idx_t limit)
{
    if (limit <= PRIMES_LIMIT)
        return;
    free(PRIMES);
    PRIMES = NULL;
    PRIMES_COUNT = 0;
    /*  build_primes_upto( limit , &PRIMES , &PRIMES_COUNT )  */
    build_primes_upto(limit, &PRIMES, &PRIMES_COUNT);
    PRIMES_LIMIT = limit;
}

/*  Main sequence function.  */
seq_t
a(idx_t n)
{
    if (n < 4)
        return 0;                /*  undefined for the offset (a(4) is first) */

    /*  We only need primes up to sqrt(4 n²) ≤ 2 n.  */
    ensure_primes(2 * n);

    uint64_t nn = (uint64_t)n;

    for (idx_t k = 1; k <= n; ++k) {
        /*  D = 4 n² – (2k–1)²  */
        uint64_t t = (uint64_t)(2 * k - 1);
        uint64_t d = 4ULL * nn * nn - t * t;
        if (d == 0)
            continue;

        uint64_t tmp = d;

        /*  Trial factorisation with the precompiled prime list.  */
        for (size_t i = 0; i < PRIMES_COUNT; ++i) {
            uint64_t p = PRIMES[i];
            if (p * p > tmp)
                break;
            if (tmp % p == 0) {
                /*  p is a prime divisor of D.  */
                if (p < 2 * nn) {
                    uint64_t diff = 2 * nn - p;
                    if (diff >= 2 && is_prime((idx_t)diff))
                        return (seq_t)k;
                }
                /*  Remove all occurrences of p from tmp.  */
                while (tmp % p == 0)
                    tmp /= p;
            }
        }

        /*  Remaining factor > 1 (otherwise tmp == 1).  */
        if (tmp > 1) {
            uint64_t p = tmp;
            if (p < 2 * nn) {
                uint64_t diff = 2 * nn - p;
                if (diff >= 2 && is_prime((idx_t)diff))
                    return (seq_t)k;
            }
        }
    }

    /*  Should never reach here for n ≥ 4.  */
    return 0;
}
