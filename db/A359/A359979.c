/*
 * A359979  Irregular table T(n,k), n >= 0 and k >= 0, read by rows with T(n +
 *          3*k,k) = A008619(n).
 * https://oeis.org/A359979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by reading an infinite irregular table * T(m,k)
 *            (rows m = 0,1,2,…) where T(m,k) = ⌊(m-3k)/2⌋+1 for all * m ≥ 3k.
 *            The table is traversed row‑wise: for each row m we list * the
 *            entries T(m,0),T(m,1),… up to k = ⌊m/3⌋. This function *
 *            computes the m and k that correspond to the inde...
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* required by the build system */

seq_t a(idx_t n)
{
    /* Find the row m such that the cumulative number of terms up to row m-1
     * is ≤ n and the number of terms up to row m is > n.
     */
    idx_t m = 0;                /* will become one past the desired row */
    idx_t cumulative = 0;       /* total terms counted so far */

    while (cumulative <= n)
    {
        idx_t len = (m / 3) + 1;   /* length of current row m */
        cumulative += len;
        ++m;
    }

    /* Now m is the first row with cumulative > n, so the desired row is m-1 */
    idx_t row = m - 1;
    idx_t len_row = (row / 3) + 1;
    idx_t prev_cumulative = cumulative - len_row;   /* terms before this row */
    idx_t k = n - prev_cumulative;                  /* column index in this row */

    /* Compute the table value: floor((row - 3*k)/2) + 1 */
    idx_t diff = row - 3 * k;        /* Guaranteed ≥ 0 */
    seq_t value = (seq_t)(diff / 2) + (seq_t)1;

    return value;
}
