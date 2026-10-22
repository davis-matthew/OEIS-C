/*
 * A349441  Dirichlet convolution of A057521 (powerful part of n) with A055615
 *          (Dirichlet inverse of n).
 * https://oeis.org/A349441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, look
 * Formula:   a(n) = Sum_{d|n} A057521(n/d) * A055615(d) (Dirichlet
 *            convolution). Multiplicative: for prime p, a(p^e) = 1 - p if e
 *            == 1 p^2 - p if e == 2 0 otherwise
 * Range:     i8 n <= 130, i16 n <= 9093, i32 n <= 9093, i64 n <= 9093,
 *            i128 n <= 9093, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute a(n) for the multiplicative function described above.
   n is 1-indexed, so a(1)=1. */
seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t temp = (uint64_t)n;

    /* Factor using trial division up to sqrt(temp) */
    for (uint64_t p = (uint64_t)2; temp > 1 && p <= temp / p; ++p) {
        if (temp % p == (uint64_t)0) {
            int exp = 0;
            while (temp % p == (uint64_t)0) {
                temp /= p;
                ++exp;
            }
            if (exp == 1) {
                result *= (seq_t)((seq_t)1 - (seq_t)p);
            } else if (exp == 2) {
                seq_t pp = (seq_t)p;
                result *= (pp * pp - pp);
            } else {
                /* exponent > 2 => factor kills the product */
                result = (seq_t)0;
                break;
            }
        }
    }

    /* If remainder > 1, it's a prime factor with exponent 1 */
    if (temp > 1 && result != (seq_t)0) {
        result *= (seq_t)((seq_t)1 - (seq_t)temp);
    }

    return result;
}
