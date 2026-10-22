/*
 * A354268  Table read by rows. T(n, k) = (n + k)^(n - 1) for n >= 1 and 0 <=
 *          k <= n, T(0, 0) = 0.
 * https://oeis.org/A354268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A354268 – Table read by rows
 *  a(n) = T(m,k) with
 *      m = floor((sqrt(8*n+1)-1)/2)
 *      k = n - m*(m+1)/2
 *  T(n,k) = (n + k)^(n - 1)
 *
 *  For n = 0, a(0) = 0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Find the row index m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2 */
    idx_t m = 0;
    while ((m * (m + 1)) / 2 <= n)
        ++m;
    --m;                                 /* last m satisfying the inequality */

    /* Column index k within that row */
    idx_t k = n - (m * (m + 1)) / 2;

    /* Special case: for the first row, T(0,0) = 0 is already handled above */
    /* For m >= 1, compute (m + k)^(m - 1) */
    seq_t base = (seq_t)(m + k);
    idx_t exp   = m - 1;                 /* exp fits in idx_t (m <= 142 for n <= 10000) */

    return seq_pow(base, exp);
}
