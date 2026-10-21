/*
 * A289311  Let f be the multiplicative function satisfying f(p^k) = (1 +
 *          p*I)^k for any prime p and k > 0 (where I^2 = -1); a(n) = the
 *          imaginary part of f(n).
 * https://oeis.org/A289311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Im(f(n)), where f is multiplicative and f(p^k) = (1 +
 *            p·i)^k for every prime p and integer k ≥ 1. The sequence starts:
 *            a(1)=0
 * Range:     i8 n <= 103, i16 n <= 8191, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   complex_mul, complex_square, mul_pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/complex_mul.h"
#include "../utils/complex_square.h"
#include "../utils/mul_pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence is defined for n ≥ 1 */

    uint64_t x = (uint64_t)n;     /* work in native 64‑bit for factoring */
    seq_t re = 1;                 /* product = 1 + 0·i */
    seq_t im = 0;

    /* Process the factor 2 separately to avoid division by zero */
    if ((x & 1) == 0) {
        uint64_t exp = 0;
        while ((x & 1) == 0) {
            x >>= 1;
            ++exp;
        }
        mul_pow_uint(&re, &im, 2, exp);
    }

    /* Process odd prime factors by trial division */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            uint64_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            mul_pow_uint(&re, &im, p, exp);
        }
    }

    /* If anything remains, it is a prime factor > sqrt(original n) */
    if (x > 1) {
        mul_pow_uint(&re, &im, x, 1);
    }

    return im;                     /* return the imaginary part of f(n) */
}
