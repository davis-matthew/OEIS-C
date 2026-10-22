/*
 * A365298  a(n) is the smallest number k such that k*n is a cubefull
 *          exponentially odd number (A335988).
 * https://oeis.org/A365298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative with a(p) = p^2, a(p^e) = p if e is even,
 *            a(p^e) = 1 if e is odd and >1 Thus a(n) = product over prime
 *            powers in n of the above
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* If n=1, product of empty set equals 1. */
    if (n <= 1) return 1;

    uint64_t num = (uint64_t)n;
    static uint64_t prime_factors[64];
    static uint64_t exponents[64];
    size_t count = 0;

    /* Factor num into primes and exponents. */
    factor_u64(num, prime_factors, exponents, &count);

    seq_t res = 1;

    for (size_t i = 0; i < count; ++i) {
        if (exponents[i] == 1) {
            /* Multiply by p^2 */
            seq_t p = (seq_t)prime_factors[i];
            res *= p * p;
        } else if ((exponents[i] & 1UL) == 0) {
            /* Multiply by p */
            res *= (seq_t)prime_factors[i];
        } /* else odd exponent >1, multiply by 1 – no action */
    }

    return res;
}
