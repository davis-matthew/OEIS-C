/*
 * A290605  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of 2/(1 + sqrt(1 - 4*k*x)).
 * https://oeis.org/A290605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k^i * Catalan(i), where (i,k) is obtained by reading the
 *            square array A(i,k) = k^i * Catalan(i) by antidiagonals
 * Range:     u8 n <= 30, u16 n <= 50, u32 n <= 113, u64 n <= 267,
 *            u128 n <= 690, big any n
 * Helpers:   catalan, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/catalan.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t low = 0, high = 1;
    while ((high * (high + 1)) / 2 <= n) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((mid * (mid + 1)) / 2 <= n) low = mid;
        else high = mid - 1;
    }
    idx_t d   = low;
    idx_t tri = d * (d + 1) / 2;
    idx_t offset = n - tri;          /* position within diagonal d */
    idx_t row = offset;              /* i = row index */
    idx_t col = d - offset;          /* k = column index */

    /* Compute col^row */
    seq_t pow_val = 1;
    for (idx_t t = 0; t < row; ++t) {
        pow_val = seq_mul(pow_val, (seq_t)col);
    }

    /* Catalan number for the row index */
    seq_t cat = catalan(row);

    return seq_mul(pow_val, cat);
}
