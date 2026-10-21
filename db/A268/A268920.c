/*
 * A268920  Denominators of the rational number triangle R(n, k) = (n^4 -
 *          30*n^2*k^2 + 60*n*k^3 - 30*k^4) / (120*n), n >= 1, k = 1, ..., n.
 * https://oeis.org/A268920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, tabl, easy
 * Formula:   a(n) = denominator of R(n,k) = (n^4 - 30 n^2 k^2 + 60 n k^3 - 30
 *            k^4)/(120 n)
 * Range:     u8 n <= 3, u16 n <= 73, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the row i that contains the n-th element in the triangular numbering
       The number of elements up to row i is i*(i+1)/2 */
    idx_t i = 1;
    while ((i * (i + 1)) / 2 < n) ++i;

    /* Column k is the offset inside the row */
    idx_t k = n - ((i - 1) * i) / 2;

    /* Convert to seq_t for arithmetic */
    seq_t si = (seq_t)i;
    seq_t sk = (seq_t)k;

    /* Compute i^2, i^4, k^2, k^3, k^4 */
    seq_t i_sq = si * si;
    seq_t i4   = i_sq * i_sq;

    seq_t k2 = sk * sk;
    seq_t k3 = k2 * sk;
    seq_t k4 = k3 * sk;

    /* Numerator terms */
    seq_t sum_pos = i4 + 60 * si * k3;           /* i^4 + 60 n k^3 */
    seq_t sum_neg = 30 * i_sq * k2 + 30 * k4;    /* 30 i^2 k^2 + 30 k^4 */

    /* Absolute value of the numerator (to avoid signed overflow) */
    seq_t abs_num = sum_pos >= sum_neg ? sum_pos - sum_neg : sum_neg - sum_pos;

    /* Denominator before reduction */
    seq_t denom = 120 * si;

    /* Reduce the fraction */
    seq_t g = gcd(abs_num, denom);
    denom = denom / g;
    return denom;
}
