/*
 * A323211  Level 1 of Pascal's pyramid. T(n, k) triangle read by rows for n
 *          >= 0 and 0 <= k <= n.
 * https://oeis.org/A323211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(N) = T(n,k) where T(n,k)=1 if n<2 or k==0 else
 *            binomial(n-2,k-1)+1 and the index N is the flattened row‑major
 *            order of the triangle. (See OEIS A323211)
 * Range:     u8 n <= 59, u16 n <= 238, u32 n <= 719, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t N)
{
    idx_t cum = 0;          /* number of terms before current row */
    idx_t n   = 0;          /* current row number */

    /* find the row n such that cum + (n+1) > N  */
    while (cum + (n + 1) <= N) {
        cum += n + 1;
        ++n;
    }

    idx_t k = N - cum;      /* column within row n */

    if (n < 2 || k == 0) {
        return (seq_t)1;
    }

    seq_t val = binomial(n - 2, k - 1);
    val += (seq_t)1;        /* add the +1 in the definition */
    return val;
}
