/*
 * A258758  Triangle T(n,k) = C(n+k-1,k)*C(2*n-1,n-k).
 * https://oeis.org/A258758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(r + k - 1, k) * C(2·r - 1, r - k) where r is the unique
 *            row index so that r·(r + 1)/2 ≤ n < (r + 1)·(r + 2)/2, and k = n
 *            - r·(r + 1)/2
 * Range:     u8 n <= 14, u16 n <= 37, u32 n <= 95, u64 n <= 308,
 *            u128 n <= 1097, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2  */
    idx_t r = 0;
    idx_t start = 0;           /* starting index of the current row */

    while (start + r + 1 <= n) {   /* row r has length r+1 */
        start += r + 1;
        ++r;
    }

    /* Column within the row */
    idx_t k = n - start;

    /* Row 0 is special because the binomial arguments would underflow. */
    if (r == 0)
        return (seq_t)1;

    seq_t val = binomial(r + k - 1, k);
    val *= binomial(2 * r - 1, r - k);
    return val;
}
