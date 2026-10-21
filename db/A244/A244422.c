/*
 * A244422  Quasi-Riordan triangle ((2-z)/(1-z), -z^2/(1-z)). Row reversed
 *          monic Chebyshev T-polynomials without vanishing columns.
 * https://oeis.org/A244422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8·n + 1) – 1)/2⌋, k = n – r·(r+1)/2
 *            with the triangle entry T(r,0) = 1 (r ≥ 1), T(0,0) = 2 T(r,k) =
 *            (–1)^k · (r/(r−k)) · binomial(r−k, k) for 0 < k ≤ r/2 T(r,k) = 0
 *            for k > r−k (trailing columns are zero)
 * Range:     i8 n <= 46, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   _gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/_gcd_seq.h"

/* --------- helper: gcd for seq_t --------- */

/* --------- helper: binomial coefficient --------- */
static seq_t _binomial(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i); /* numerator part   */
        seq_t den = (seq_t)i;           /* denominator part */
        seq_t g1  = _gcd_seq(num, den);
        num /= g1;
        den /= g1;
        seq_t g2  = _gcd_seq(res, den);
        res /= g2;
        den /= g2;
        res = res * num;                /* remaining division exact */
        /* den should now be 1; ignore if 0 */
    }
    return res;
}

/* --------- helper: compute T(n,k) --------- */
static seq_t _T_value(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (n == 0) return (k == 0) ? (seq_t)2 : (seq_t)0;
    if (k == 0) return (seq_t)1;
    if (k > n - k) return (seq_t)0;     /* trailing zero columns */

    seq_t binom = _binomial(n - k, k);
    seq_t res   = binom * (seq_t)n;     /* multiply by n */
    res /= (seq_t)(n - k);              /* divide by (n-k) */
    if (k & 1) res = -res;              /* apply sign (–1)^k */
    return res;
}

/* --------- main sequence function --------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;        /* a(0) = 2 */

    /* find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    long double d = sqrtl((long double)8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);
    /* correct possible round‑off */
    while (((unsigned __int128)r * (r + 1)) / 2 > n) r--;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n) r++;

    idx_t offset = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t k = n - offset;

    return _T_value(r, k);
}
