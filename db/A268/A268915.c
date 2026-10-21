/*
 * A268915  Numerators of the rational number triangle R(m, a) = - (m^2 -
 *          6*m*a + 6*a^2)/(12*m), m >= 1, a = 1, ..., m. This is a
 *          regularized Sum_{j >= 0} (a + m*j) defined by analytic
 *          continuation of a generalized Hurwitz zeta function.
 * https://oeis.org/A268915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac, tabl, easy
 * Formula:   a(n) = numerator of -(m^2 - 6*m*k + 6*k^2)/(12*m), where (m, k)
 *            are the row and column indices in the triangle R(m,k) found by
 *            mapping the triangular number sequence: n = (m-1)*m/2 + k, 1 ≤ k
 *            ≤ m
 * Range:     i8 n <= 56, i16 n <= 1275, i32 n <= 1275, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The required gcd function is supplied by the build environment.
   It operates on the seq_t type. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find row m such that T_{m-1} < n ≤ T_m, where T_m = m(m+1)/2 */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t m = low;                     /* row index (1‑based) */

    /* Column index k (a) in that row */
    __int128 prev_T = (__int128)(m - 1) * m / 2;
    idx_t k = n - (idx_t)prev_T;        /* 1 ≤ k ≤ m */

    /* Convert to seq_t for arithmetic */
    seq_t M = (seq_t)m;
    seq_t K = (seq_t)k;

    /* Compute numerator and denominator of R(m,k) */
    seq_t N = -(M * M - 6 * M * K + 6 * K * K);   /* numerator before reduction */
    seq_t D = (seq_t)12 * M;                      /* denominator */

    /* Reduce by gcd and return the resulting numerator */
    seq_t absN = (N >= 0) ? N : -N;
    seq_t g = gcd(absN, D);
    return N / g;
}
