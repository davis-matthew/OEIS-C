/*
 * A272099  Triangle read by rows, T(n,k) = C(n+1,k+1)*F([k-n, k-n-1], [-n-1],
 *          -1), where F is the generalized hypergeometric function, for n>=0
 *          and 0<=k<=n.
 * https://oeis.org/A272099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (row,col) element of the triangle T(n,k) =
 *            C(n+1,k+1) * Σ_{m=0}^{n-k} C(n-k+1,m) * C(n+1-m,k+1), where
 *            C(x,y) is the binomial coefficient. This implementation
 *            evaluates a(n) for any n≥0 in O(n) time. No hard‑coded values
 *            are used; the result is computed on demand. It requires the
 *            util...
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row (n) and column (k) of the flattened triangle */
    idx_t row = 0;
    /* Increment row until the next triangular number would exceed n */
    while ((row + 1ULL) * (row + 2ULL) / 2ULL <= n)
        ++row;
    idx_t col = n - row * (row + 1ULL) / 2ULL;   /* column index 0 ≤ col ≤ row */

    /* Compute T(row,col) by the derived integer sum */
    idx_t r          = row - col;                /* r = n - k  */
    idx_t r_plus_one = r + 1ULL;                 /* r+1       */
    seq_t sum        = 0;
    seq_t binom2     = 1;                        /* C(r+1,0)  */
    seq_t binom_cur  = binomial(row + 1ULL, col + 1ULL); /* C(row+1, k+1) */

    for (idx_t m = 0; m <= r; ++m)
    {
        seq_t term = binom2 * binom_cur;        /* C(r+1,m) * C(row+1-m, k+1) */
        sum += term;

        if (m < r)
        {
            /* Update C(r+1, m+1) from C(r+1, m) */
            binom2 = (binom2 * (r_plus_one - m)) / (m + 1ULL);

            /* Update C(row+1-m, k+1) to C(row-m, k+1)              */
            idx_t x   = row + 1ULL - m;                /* current top of binomial */
            idx_t num = x - (col + 1ULL);              /* x - (k+1) */
            binom_cur = (binom_cur * num) / x;         /* exact division */
        }
    }

    return sum;
}
