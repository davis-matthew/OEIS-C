/*
 * A327923  Irregular triangle read by rows: Coefficients of Schick's
 *          polynomials P(n, y^2), for n >= 1.
 * https://oeis.org/A327923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 24, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A327923 – Irregular triangle read by rows: coefficients of Schick's
 *  polynomials P(n, y^2).  For n ≥ 1
 *
 *  t(r,k) = (-1)^(r-1 + (2^(r-1)-1-k)) *
 *           binom(2^(r-1)+k , 2^(r-1)-1-k) *
 *           2^(1 + 2k - r)
 *
 *  a(n) is the flattening of the triangle, i.e.
 *     r = ⌈log₂(n+1)⌉               (1 ≤ r)
 *     k = n - 2^(r-1)               (0 ≤ k < 2^(r-1))
 *  Then a(n) = t(r,k).
 *
 *  The routine below calculates a(n) for any n ≥ 1 without
 *  pre‑computing the whole triangle.  It uses a multiplicative
 *  binomial algorithm with gcd‑reduction to keep intermediate
 *  values bounded, which works for both fixed‑width and
 *  arbitrary‑precision seq_t types.  Shifts are applied using
 *  a signed exponent so that negative shifts (division by powers
 *  of two) are handled correctly even for unsigned idx_t.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"   /*  seq_t gcd(seq_t a, seq_t b)   */

static seq_t
binomial(idx_t n, idx_t k)
{
    /* compute C(n,k) exactly using gcd‑reduction */
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;           /* use smaller side */

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        seq_t g1 = gcd(res, den);          /* cancel with current result */
        if (g1 > 1) {
            res /= g1;
            den /= g1;
        }
        seq_t g2 = gcd(num, den);          /* cancel with denominator */
        if (g2 > 1) {
            num /= g2;
            den /= g2;
        }
        res *= num;                        /* now division is exact */
        if (den > 1) res /= den;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* only defined for n ≥ 1; behaviour for n==0 is undefined here */
    if (n == 0) return (seq_t)0;

    /* r = ⌈log₂(n+1)⌉  =  64 - __builtin_clzll(n) */
    idx_t r = 64 - __builtin_clzll(n);          /* r ≥ 1 */
    idx_t p = (idx_t)1 << (r - 1);             /* 2^(r-1)     */
    idx_t k = n - p;                           /* 0 ≤ k < p   */

    /* parameters for the binomial coefficient */
    idx_t y_bin = p - 1 - k;                   /* 2^(r-1)-1-k  */
    idx_t N = p + k;                           /* p+k          */

    seq_t bin = binomial(N, y_bin);            /* C(N, y_bin)  */

    /* exponent for the power of two factor: exp = 1 + 2k - r   (signed) */
    int64_t exp = (int64_t)1 + 2 * (int64_t)k - (int64_t)r;

    /* apply the power of two factor */
    if (exp > 0) {
        /* shift left exp times; exp is small enough for 64‑bit types */
        for (int64_t i = 0; i < exp; ++i) bin <<= 1;
    } else if (exp < 0) {
        /* shift right to divide by 2^(-exp) */
        for (int64_t i = 0; i < -exp; ++i) bin >>= 1;
    }

    /* sign: (-1)^(r-1 + y_bin) */
    bool negative = (((r - 1) + y_bin) & 1) != 0;
    if (negative) bin = -bin;

    return bin;
}
