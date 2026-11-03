/*
 * A344336  Number of divisors of n^9.
 * https://oeis.org/A344336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of divisors of n^9 (a(p^e) = 9*e+1)
 * Range:     u8 n <= 23, u16 n <= 2309, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9391 (2026-11-01)
 * Helpers:   build_primes_upto, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/isqrt.h"

static uint64_t *primes   = NULL;      /* array of primes up to current limit */
static size_t   primes_cnt   = 0;      /* number of primes stored */
static idx_t    primes_max   = 0;      /* maximum limit up to which primes have been generated */

/* Return the number of divisors of n^9 */
seq_t a(idx_t n)
{
    /* Ensure we have primes up to floor(sqrt(n)) */
    idx_t limit = isqrt(n);
    if (limit > primes_max) {
        /* Rebuild prime table up to the new limit */
        free(primes);
        build_primes_upto(limit, &primes, &primes_cnt);
        primes_max = limit;
    }

    seq_t result = (seq_t)1;
    uint64_t nn = (uint64_t)n;          /* local copy for factoring */

    /* Trial division by primes */
    for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
        uint64_t p = primes[i];
        if (nn % p == 0) {
            idx_t exp = 0;
            while (nn % p == 0) {
                nn /= p;
                ++exp;
            }
            /* Multiply by (9*exp + 1) */
            seq_t mult = (seq_t)(9 * exp + 1);
            result *= mult;
        }
    }

    /* If a prime factor remains (greater than sqrt(n)) */
    if (nn > 1) {
        result *= (seq_t)10;          /* e = 1 => 9*1 + 1 = 10 */
    }

    return result;
}
