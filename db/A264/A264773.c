/*
 * A264773  Triangle T(n,k) = binomial(4*n - 3*k, 3*n - 2*k), 0 <= k <= n.
 * https://oeis.org/A264773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the flattened triangle with entries T(r,k) =
 *            binomial(4r-3k, r-k), 0 ≤ k ≤ r. The row index r is given by r =
 *            floor((sqrt(8*n+1)-1)/2), and the column index k is k = n -
 *            r*(r+1)/2
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 65, u64 n <= 230, u128 n <= 860,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = (idx_t)((sqrt((long double)(8*n + 1)) - 1.0L) / 2.0L);

    /* Adjust in case of floating‑point rounding errors */
    while ((r+1)*(r+2)/2 <= n) ++r;
    while (r*(r+1)/2 > n) --r;

    idx_t k = n - r*(r+1)/2;

    idx_t N = 4*r - 3*k;
    idx_t K = r - k;

    return binom(N, K);
}
