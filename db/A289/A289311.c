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
 * Validated: standard, pass, 79 terms through n = 9515 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply two complex integers (a+bi)*(c+di).  Results are
   returned through *re and *im.  All arithmetic is carried out
   in the type seq_t, as required by the build. */
static void complex_mul(seq_t *re, seq_t *im, seq_t a, seq_t b, seq_t c, seq_t d)
{
    seq_t nr = (*re) * a - (*im) * b;
    seq_t ni = (*re) * b + (*im) * a;
    *re = nr;
    *im = ni;
}

/* Square a complex integer (a+bi).   Result returned in *re, *im. */
static void complex_square(seq_t *re, seq_t *im)
{
    seq_t a = *re, b = *im;
    seq_t nr = a * a - b * b;            /* (a+bi)^2 = (a^2-b^2)+(2ab)i */
    seq_t ni = a * b + b * a;             /* 2ab */
    *re = nr;
    *im = ni;
}

/* Compute (1 + p·i)^k using binary exponentiation.  The result
   is multiplied into the running product currently in *re, *im. */
static void mul_pow_uint(seq_t *re, seq_t *im, uint64_t p, uint64_t k)
{
    seq_t base_re = 1;
    seq_t base_im = (seq_t)p;
    uint64_t e = k;
    while (e) {
        if (e & 1) {
            complex_mul(re, im, base_re, base_im, *re, *im);
        }
        complex_square(&base_re, &base_im);
        e >>= 1;
    }
}

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
