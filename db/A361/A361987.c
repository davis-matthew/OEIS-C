/*
 * A361987  a(1) = 1; a(n) = n^2 * Sum_{d|n, d < n} (-1)^(n/d) a(d) / d^2.
 * https://oeis.org/A361987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = n^2 * Sum_{d|n, d < n} (-1)^(n/d) a(d) / d^2 Computation:
 *            a is multiplicative. * a(1) = 1. * For n = 2^e: a(n) = 2^(3e-1).
 *            * For odd prime p: a(p) = -p^2. * For odd prime powers p^e, e >
 *            1: a(p^e) = 0. The product over prime power factors gives a(n)
 * Range:     i8 n <= 7, i16 n <= 63, i32 n <= 2047, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   pow2_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2_small.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t temp = n;

    /* handle powers of 2 */
    idx_t exp2 = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        exp2++;
    }
    if (exp2 > 0) {
        /* 2^(3*exp2 - 1) */
        result *= pow2_small(3 * exp2 - 1);
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if (exp == 1) {
                seq_t pp = (seq_t)p * (seq_t)p;
                result *= -(seq_t)pp;
            } else {
                /* odd prime power with exponent > 1 => a(n)=0 */
                return 0;
            }
        }
    }

    /* remaining prime factor > 1 (must be odd prime exponent 1) */
    if (temp > 1) {
        seq_t pp = (seq_t)temp * (seq_t)temp;
        result *= -(seq_t)pp;
    }

    return result;
}
