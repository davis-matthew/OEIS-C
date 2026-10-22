/*
 * A359834  Parity of Dirichlet inverse of A359832, where A359832(n) = 1 if
 *          the 2-adic valuation of n is either 0 or odd, otherwise 0.
 * https://oeis.org/A359834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 iff n is odd‑squarefree (every odd prime divides n
 *            exactly once) and the exponent of 2, if present, is positive and
 *            not a multiple of 3. Otherwise a(n)=0. This follows from the
 *            multiplicative definition of the Dirichlet inverse of A359832
 * Range:     u8 n <= 9721, u16 n <= 9721, u32 n <= 9721, u64 n <= 9721,
 *            u128 n <= 9721, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    /* 1 is a unit; its factorisation is empty, so return 1. */
    if (m == 1)
        return (seq_t)1;

    /* Temporary arrays for prime factors and exponents. */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt;

    factor_u64(m, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (p == 2) {
            /* 2-adic exponent must not be a positive multiple of 3. */
            if (e > 0 && (e % 3) == 0)
                return (seq_t)0;
        } else {
            /* Odd primes must appear to the first power only. */
            if (e > 1)
                return (seq_t)0;
        }
    }
    return (seq_t)1;
}
