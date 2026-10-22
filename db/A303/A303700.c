/*
 * A303700  Triangle read by rows in which row n gives coefficients of
 *          polynomial f_n(x)/(n+1) of degree less than n that satisfies
 *          Integral_{x=0..1} g(t - x) * f_n(x) dx = g(t) for any polynomial
 *          g(x) of degree less than n.
 * https://oeis.org/A303700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(m,k) where m is the row index such that m(m+1)/2 <= n <
 *            (m+1)(m+2)/2 and k = n - m(m+1)/2. T(m,k) = (-1)^k * (m+k+1)! *
 *            (k+1) / ((k+1)!^2 * (m-k)!). Recurrence: T(m,0) = m+1; T(m,i) =
 *            -T(m,i-1) * (m+i+1)(m-i+1) / (i(i+1))
 * Range:     i8 n <= 11, i16 n <= 32, i32 n <= 112, i64 n <= 394,
 *            i128 n <= 1461, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = 0;
    /* find row index m such that m(m+1)/2 <= n < (m+1)(m+2)/2 */
    while (((unsigned __int128)(m + 1) * (m + 2)) / 2 <= n) {
        ++m;
    }

    idx_t cum_prev = (idx_t)(((unsigned __int128)m * (m + 1)) / 2);
    idx_t k = n - cum_prev;            /* position within the row */

    seq_t t = (seq_t)(m + 1);          /* T(m,0) */

    for (idx_t i = 1; i <= k; ++i) {
        idx_t num_val = (m + i + 1) * (m - i + 1);
        idx_t den_val = i * (i + 1);
        seq_t num = (seq_t)num_val;
        seq_t den = (seq_t)den_val;

#ifdef __cplusplus
        t = (seq_t)(-t * num / den);
#else
        /* use 128‑bit intermediate to avoid overflow for integral types */
        __int128 prod = (__int128)t * (__int128)num;
        prod /= den;
        t = (seq_t)(-prod);
#endif
    }

    return t;
}
