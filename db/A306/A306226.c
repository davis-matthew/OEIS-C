/*
 * A306226  Triangle read by rows: T(n,k) = Sum_{i=0..n/2}
 *          C(n-i,i)*C(n-i,k-i)*C(n-1,i) (0 <= k <= n).
 * https://oeis.org/A306226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) corresponds to the triangle entry T(r,k) with r =
 *            floor((sqrt(8n+1)-1)/2) (the row index), k = n - r*(r+1)/2 (the
 *            column index in that row), and T(r,k) = Σ_{i=0..⌊r/2⌋} C(r-i,i)
 *            * C(r-i,k-i) * C(r-1,i) (C(n,k) = binomial coefficient, with
 *            C(n,k)=0 if k>n or k<0)
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the trivial first term */
    if (n == 0)
        return 1;                       /* T(0,0) is defined as 1 */

    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (1) {
        idx_t next = r + 1;
        /* sum of rows up to 'next-1'th index = next*(next+1)/2 */
        idx_t cum = next * (next + 1) / 2;
        if (n < cum)
            break;
        r = next;
    }

    /* Column index within the row */
    idx_t k = n - r * (r + 1) / 2;
    if (r == 0)                      /* should only happen for n==0 handled above */
        return 1;

    /* Limit of the summation index i:
       i ranges 0..⌊r/2⌋, but we also need i <= k to keep C(r-i,k-i) valid. */
    idx_t i_limit = r / 2;
    if (k < i_limit)
        i_limit = k;

    seq_t sum = 0;
    for (idx_t i = 0; i <= i_limit; ++i) {
        seq_t term = binomial(r - i, i);
        term *= binomial(r - i, k - i);
        term *= binomial(r - 1, i);
        sum += term;
    }
    return sum;
}
