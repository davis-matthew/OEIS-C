/*
 * A352967  Array read by antidiagonals: A(i, j) = abs(i - j) if min(i, j) <
 *          max(i, j) <= 2*min(i, j), and otherwise 0, with i >= 0 and j >= 0.
 * https://oeis.org/A352967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(i,j) where k = floor((sqrt(8*n+1)-1)/2) is the diagonal
 *            index, t = n - k*(k+1)/2 (0 ≤ t ≤ k), i = t, j = k - t. A(i,j) =
 *            abs(i-j) iff min(i,j) < max(i,j) ≤ 2*min(i,j), else 0
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find diagonal index k and offset t */
    idx_t k = 0;
    idx_t cum = 0;          /* cumulative terms before this diagonal */
    idx_t len = 1;          /* length of current diagonal */

    while (cum + len <= n) {
        cum += len;
        ++k;
        ++len;
    }
    idx_t t = n - cum;           /* position within diagonal */
    idx_t i = t;
    idx_t j = k - t;

    /* Compute value using the definition */
    if (i == j) return (seq_t)0;

    idx_t m = i < j ? i : j;     /* min */
    idx_t M = i < j ? j : i;     /* max */

    /* Conditions: m < M and M <= 2*m */
    if (m < M && M <= 2 * m) {
        return (seq_t)(M - m);
    } else {
        return (seq_t)0;
    }
}
