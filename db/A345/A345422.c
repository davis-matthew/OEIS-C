/*
 * A345422  Row 5 of array in A345417.
 * https://oeis.org/A345422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = u, where 11*u + p*v = 1 for p = prime(n) (p != 11), with
 *            u the residue of 11^(-1) mod p of least absolute value (u = 1
 *            for p = 2); a(5) = 0 since p = 11
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-10)
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/nth_prime.h"

seq_t a(idx_t n)
{
    int64_t p = (int64_t)nth_prime(n);
    if (p == 11) return (seq_t)0;
    int64_t m = 11 % p;
    /* extended Euclid for inverse of m modulo p */
    int64_t r0 = p, r1 = m, t0 = 0, t1 = 1;
    while (r1 != 0) {
        int64_t q = r0 / r1;
        int64_t r2 = r0 - q * r1;
        int64_t t2 = t0 - q * t1;
        r0 = r1; r1 = r2;
        t0 = t1; t1 = t2;
    }
    int64_t inv = t0 % p;
    if (inv < 0) inv += p;
    if (2 * inv > p) inv -= p;
    return (seq_t)inv;
}
