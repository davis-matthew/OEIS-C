/*
 * A319047  Square array A(n,k) where column k is balanced (2k+1)-ary
 *          enumeration of integers; n>=0, k>=1, read by antidiagonals.
 * https://oeis.org/A319047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, hear, tabl
 * Formula:   For n >= 0, let (i,k) be the unique pair with i>=0, k>=1 such
 *            that n = T(t-1) + i where t = i + k and T(m) = m(m+1)/2. Then
 *            a(n) is the i-th integer in the balanced (2k+1)-ary enumeration,
 *            i.e. write i in base (2k+1), replace every digit d by d' = d (if
 *            d<=k) or d' = d-(2k+1) (if d>k), and interp...
 * Range:     i8 n <= 8775, i16 n <= 9312, i32 n <= 9312, i64 n <= 9312,
 *            i128 n <= 9312, big any n
 * Helpers:   balanced_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/balanced_value.h"

seq_t a(idx_t n)
{
    /* Find t such that T(t-1) <= n < T(t), where T(m)=m(m+1)/2. */
    idx_t t = 0;
    idx_t sum = 0;
    while (sum <= n) {
        t++;
        sum += t;
    }

    /* prev_sum = T(t-1) */
    idx_t prev_sum = sum - t;
    idx_t d = n - prev_sum;      /* offset within the t-th diagonal */
    idx_t i = d;                 /* row index */
    idx_t k = t - d;             /* column index (>=1) */

    return balanced_value(i, k);
}
