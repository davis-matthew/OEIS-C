/*
 * A326689  Numerator of the fraction (Sum_{prime p | n} 1/p - 1/n).
 * https://oeis.org/A326689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of ( Σ_{prime p|n} 1/p – 1/n )
 * Range:     i8 n <= 13, i16 n <= 254, i32 n <= 9335, i64 n <= 9335,
 *            i128 n <= 9335, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Computes the numerator of (∑_{p|n} 1/p – 1/n) in reduced form.
   Works for all n ≥ 1. */
seq_t a(idx_t n)
{
    seq_t num = 0;      /* current numerator of the sum over primes */
    seq_t den = 1;      /* current denominator (always positive) */
    uint64_t temp = n;  /* value to factor */

    /* handle factor 2 separately for speed */
    if (temp % 2 == 0) {
        seq_t pseq = 2;
        seq_t num_new = num * pseq + den;
        seq_t den_new = den * pseq;
        seq_t g = gcd(num_new < 0 ? -num_new : num_new, den_new);
        num = num_new / g;
        den = den_new / g;
        while (temp % 2 == 0)
            temp /= 2;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            seq_t pseq = p;
            seq_t num_new = num * pseq + den;
            seq_t den_new = den * pseq;
            seq_t g = gcd(num_new < 0 ? -num_new : num_new, den_new);
            num = num_new / g;
            den = den_new / g;
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* remaining prime factor > 1 */
    if (temp > 1) {
        seq_t pseq = (seq_t)temp;
        seq_t num_new = num * pseq + den;
        seq_t den_new = den * pseq;
        seq_t g = gcd(num_new < 0 ? -num_new : num_new, den_new);
        num = num_new / g;
        den = den_new / g;
    }

    /* subtract 1/n: (num/den) – 1/n = (num*n – den) / (den*n) */
    seq_t nseq = (seq_t)n;
    seq_t num_new = num * nseq - den;
    seq_t den_new = den * nseq;
    seq_t g = gcd(num_new < 0 ? -num_new : num_new, den_new);
    num = num_new / g;
    /* den = den_new / g;  /* denominator is unused after this point */

    return num;
}
