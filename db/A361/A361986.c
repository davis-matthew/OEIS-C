/*
 * A361986  a(1) = 1, a(2) = 3; a(n) = n^2 * Sum_{d|n, d < n} (-1)^(n/d) a(d)
 *          / d^2.
 * https://oeis.org/A361986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = n^2 * Sum_{d|n, d < n} (-1)^(n/d) a(d) / d^2 The sequence
 *            is multiplicative with a(1) = 1 a(2) = 3 a(2^e) = 7 * 2^(3*e-4)
 *            (e > 1) a(p) = -p^2 (p odd prime) a(p^e) = 0 (p odd prime, e >
 *            1) Hence a(n) = 0 if n has any odd prime factor with exponent >
 *            1. For other n the value is the product o...
 * Range:     i8 n <= 7, i16 n <= 63, i32 n <= 2047, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/pow2.h"          /* pow2 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* 64-bit primes and exponents of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term = 1;

        if (p == 2) {
            if (e == 1) {
                term = 3;
            } else {
                /* a(2^e) = 7 * 2^(3e-4)  for e > 1   */
                seq_t pow = pow2((idx_t)(3 * e - 4));
                term = 7 * pow;
            }
        } else { /* odd prime */
            if (e == 1) {
                term = - (seq_t)p * (seq_t)p;
            } else {
                /* any odd prime power >1 gives zero */
                return 0;
            }
        }

        result *= term;
    }

    return result;
}
