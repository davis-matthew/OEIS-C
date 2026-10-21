/*
 * A293472  Triangle read by rows, coefficients of polynomials in t = log(x)
 *          of the n-th derivative of x^x, evaluated at x = 1. T(n, k) with n
 *          >= 0 and 0 <= k <= n.
 * https://oeis.org/A293472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 28, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A293472 – Flattened triangle of the n‑th derivative of x^x at x=1.
 *  The value a(n) equals binom(r,k) * T(r-k,0) where
 *      r = ⌊(√(8n+1)-1)/2⌋          (the row containing index n)
 *      k = n - r*(r+1)/2              (column within that row)
 *  T(m,0) (the constant term of the m‑th derivative) is the m‑th Bell
 *  polynomial B_m(1,1,-1,2,-6,24,…) evaluated at the arguments
 *  g^(p)(1)=(-1)^p (p‑2)! for p≥2.
 *  The code computes T(m,0) by the recurrence
 *       T(0)=1,
 *       T(m)=Σ_{k=1}^{m} binom(m-1,k-1) * x_k * T(m-k),
 *  where x_1=x_2=1 and for k≥3, x_k = (-1)^k * (k-2)! .
 *  All arithmetic is performed in the integer type seq_t.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    /* Map flat index n to row r and column k */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* Static cache for T(m,0) up to the largest m needed */
    static seq_t t0[200] = {0};
    static idx_t cached = 0;
    static bool init = false;
    static seq_t fact[200] = {0};
    static bool fact_init = false;

    /* Initialise factorial table once */
    if (!fact_init) {
        fact[0] = (seq_t)1;
        for (idx_t i = 1; i < 200; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        fact_init = true;
    }

    /* Initialise base value for T(0,0) */
    if (!init) {
        t0[0] = (seq_t)1;
        init = true;
    }

    /* Compute T(m,0) up to r if not yet done */
    if (cached < r) {
        for (idx_t m = cached + 1; m <= r; ++m) {
            seq_t sum = 0;
            for (idx_t kk = 1; kk <= m; ++kk) {
                seq_t bin = binom(m - 1, kk - 1);
                seq_t xk;
                if (kk <= 2) {
                    xk = (seq_t)1;
                } else {
                    seq_t f = fact[kk - 2];
                    xk = (kk & 1u) ? -f : f;   /* (-1)^kk * (kk-2)!  */
                }
                seq_t term = bin * xk * t0[m - kk];
                sum += term;
            }
            t0[m] = sum;
        }
        cached = r;
    }

    /* Return the requested triangle entry */
    return binom(r, k) * t0[r - k];
}
