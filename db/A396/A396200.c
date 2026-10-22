/*
 * A396200  Triangle read by rows: T(n, k) = Sum_{i=0..n} binomial(2*i, i) *
 *          binomial(n - 2*i, k - i).
 * https://oeis.org/A396200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Triangle(row, col) where row = largest r with r(r+1)/2 ≤
 *            n, col = n – r(r+1)/2, and Triangle(r, c) = Σ_{i=0}^{max(c,
 *            r−c)} binomial(r+1,i)
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r and column k of the flattened sequence. */
    idx_t r = 0, cum = 0;                /* cum = number of terms in rows < r */
    while (cum + r + 1 <= n) {
        cum += r + 1;                    /* add terms of current row */
        ++r;                             /* move to next row */
    }
    idx_t k = n - cum;                    /* column within row r */

    /* Determine the summation limit m = max(k, r−k). */
    idx_t m = (k > r - k) ? k : r - k;
    idx_t N = r + 1;                      /* N = r+1 in the binomial entries */

    /* Compute the sum of binomials. */
    seq_t sum = binomial(N, 0);
    for (idx_t i = 1; i <= m; ++i) {
        sum += binomial(N, i);
    }
    return sum;
}
