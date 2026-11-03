/*
 * A267863  Numerators of the rational number triangle R(m, a) = (m -
 *          2*a)/(2*m), m >= 1, a = 1, ..., m. This is a regularized Sum_{j >=
 *          0} (a + m*j)^(-s) for s = 0 defined by analytic continuation of a
 *          generalized Hurwitz Zeta function.
 * https://oeis.org/A267863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac, tabl, easy
 * Formula:   a(n) = (m-2a) / gcd(|m-2a|, 2m), where m is the row index * and
 *            a is the column index obtained from n by the usual triangular *
 *            mapping: n lies between T(m-1)+1 and T(m), with T(k)=k(k+1)/2
 * Range:     i8 n <= 7, i16 n <= 1906, i32 n <= 5050, i64 n <= 5050,
 *            i128 n <= 5050, big any n
 * Validated: standard, pass, 94 terms through n = 5008 (2026-11-01)
 * Helpers:   gcd, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Compute m from n: smallest m such that m(m+1)/2 >= n. */
    seq_t s = floor_sqrt((seq_t)8 * (seq_t)n + 1);
    seq_t m = (s - 1) / 2;              // floor of (sqrt(8n+1)-1)/2
    if (m * (m + 1) / 2 < (seq_t)n)     // adjust if not enough
        m++;

    /* Column index within row m */
    seq_t t_prev = m * (m - 1) / 2;      // T(m-1)
    seq_t a_col = (seq_t)n - t_prev;     // a = n - T(m-1)

    /* Numerator and denominator before reduction */
    seq_t num = m - 2 * a_col;
    seq_t den = 2 * m;

    /* Reduce the fraction and return the numerator */
    seq_t abs_num = num >= 0 ? num : -num;
    seq_t g = gcd(abs_num, den);
    return num / g;
}
