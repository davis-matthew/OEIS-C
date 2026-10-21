/*
 * A281297  Triangular array of generalized Narayana numbers T(n,k) =
 *          4*binomial(n+1,k)* binomial(n-4,k-1)/(n+1) for n >= 3 and 0 <= k
 *          <= n-3, read by rows.
 * https://oeis.org/A281297
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the flattening of the triangular array of generalized
 *            Narayana numbers T(n,k) = 4 * binomial(n+1,k) *
 *            binomial(n-4,k-1) / (n+1), with n ≥ 3 and 0 ≤ k ≤ n‑3. The
 *            sequence starts with a(3)=1
 * Range:     u8 n <= 14, u16 n <= 42, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                     /* undefined for n < 3; value ignored */

    /* zero‑based position in the flattened array */
    idx_t idx = n - 3;

    /* find the row such that cumulative(row-1) ≤ idx < cumulative(row) */
    idx_t row = 3;
    while (((row - 2) * (row - 1)) / 2 <= idx)
        ++row;

    /* number of elements before this row */
    idx_t cum_before = ((row - 3) * (row - 2)) / 2;
    idx_t k = idx - cum_before;        /* column position within the row */

    /* special treatment for the leading column */
    if (k == 0) {
        return row == 3 ? (seq_t)1 : (seq_t)0;
    }

    /* compute the generalized Narayana number for k > 0 */
    seq_t bin1 = binomial(row + 1, k);        /* binomial(n+1,k) */
    seq_t bin2 = binomial(row - 4, k - 1);    /* binomial(n-4,k-1) */

    seq_t res = bin1;
    res *= bin2;              /* product of the two binomials */
    res *= (seq_t)4;          /* multiply by 4 */
    res /= (seq_t)(row + 1);  /* divide by (n+1) */

    return res;
}
