/*
 * A364361  Table read by rows. T(n, k) = Sum_{j=0..n-k} k*binomial(k,
 *          j)*binomial(n - j, k).
 * https://oeis.org/A364361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, k) where * row = largest r with r(r+1)/2 <= n, * k
 *            = n - r(r+1)/2, * T(r,k) = Σ_{j=0}^{r-k} k * binomial(k,j) *
 *            binomial(r-j,k)
 * Range:     u8 n <= 30, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0)
        return 0;

    /* Find the row containing index n: r such that
     * r(r+1)/2 <= n < (r+1)(r+2)/2   */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Column index within the row */
    idx_t k = n - (r * (r + 1)) / 2;

    /* If k == 0, the whole sum is zero */
    if (k == 0)
        return 0;

    seq_t sum = 0;
    /* Compute the sum T(r,k) */
    for (idx_t j = 0; j <= r - k; ++j)
    {
        seq_t term = (seq_t)k * binom(k, j) * binom(r - j, k);
        sum += term;
    }

    return sum;
}
