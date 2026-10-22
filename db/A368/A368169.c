/*
 * A368169  The number of divisors of the largest unitary divisor of n that is
 *          a cubefull exponentially odd number (A368167).
 * https://oeis.org/A368169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative with a(p^e) = e+1 when e is odd and >1,
 *            otherwise a(p^e) = 1. Equivalently, it is the divisor‑count of
 *            the largest unitary divisor of n that is cubefull exponentially
 *            odd
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* arrays for prime factors and exponents; 20 primes is more than enough
       for 64‑bit integers.                                                         */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        if ((e > 1) && (e & 1U)) {        /* e odd and >1 */
            result *= (seq_t)(e + 1);
        }
    }
    return result;
}
