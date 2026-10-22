/*
 * A332099  Square array T(n,k) = k^n - Sum_{0 < i < k} e(i)*(k-i)^n where
 *          e(i) = 1 if the partial sum up to this term would remain <= k^n,
 *          or 0 else; n, k >= 1; read by falling antidiagonals.
 * https://oeis.org/A332099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained from the square array T(i,j) with i as the
 *            first coordinate (row) and j as the second (column), where
 *            T(i,j) = j^i - Σ_{t=1}^{j-1} e(t)·(j-t)^i , and e(t) = 1 iff the
 *            cumulative subtraction would not exceed j^i. The sequence a(n)
 *            is read by falling antidiagonals of T(i,j)
 * Range:     u8 n <= 24, u16 n <= 61, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

static seq_t greedy_T(idx_t row, idx_t col)
{
    /* row ≥ 1, col ≥ 1 */
    /* Compute j^i first, then subtract larger powers first. */
    seq_t remain = big_pow((seq_t)col, (seq_t)row);

    /* Descending j from col-1 down to 1 */
    idx_t jj = col;
    while (jj > 1) {
        jj--;                       /* jj now in [1, col-1] */
        seq_t power = big_pow((seq_t)jj, (seq_t)row);
        if (remain >= power)
            remain -= power;
    }
    return remain;
}

seq_t a(idx_t n)
{
    if (n == 0) /* not defined for offset 1, return 0 as safety */
        return 0;

    /* Find the falling antidiagonal's sum index s such that
       C(s) = s(s-1)/2 >= n and C(s-1) < n. */
    idx_t s = 1;
    while ((s * (s - 1)) / 2 < n)
        s++;

    /* Previous cumulative count */
    idx_t prev = ((s - 1) * (s - 2)) / 2;

    /* Offset within this antidiagonal (0‑based) */
    idx_t offset = n - prev - 1;

    /* Coordinates: i = offset+1, j = s - i */
    idx_t i = offset + 1;          /* row index */
    idx_t j = s - i;               /* column index */

    return greedy_T(i, j);
}
