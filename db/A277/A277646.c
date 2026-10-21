/*
 * A277646  Triangle T(n,k) = floor(n^2/k) for 1 <= k <= n^2, read by rows.
 * https://oeis.org/A277646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 1240, u16 n <= 9786, u32 n <= 9786, u64 n <= 9786,
 *            u128 n <= 9786, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A277646: Triangle T(n,k) = floor(n^2/k) read by rows.
 *  The sequence a(n) is the concatenation of each row.
 *  For a given index n (1‑based) we find the row r such that
 *       sum_{i=1}^{r-1} i^2 < n <= sum_{i=1}^r i^2
 *  let p = n - sum_{i=1}^{r-1} i^2   (position in that row).
 *  Then a(n) = floor(r^2 / p).
 *  This implementation is O(sqrt(n)) per call and works for any
 *  n reachable within the limits of seq_t and idx_t.
 */

seq_t a(idx_t n)
{
    idx_t target = n;                       /* 1‑based target index   */
    idx_t cumulative = 0;                   /* indices covered so far */
    idx_t row = 1;                          /* current row (n >= 1)   */

    while (1)
    {
        idx_t row_len = row * row;          /* length of current row  */

        if (cumulative + row_len >= target)
        {
            idx_t pos = target - cumulative;          /* 1‑based position in row */
            seq_t numerator = (seq_t)row * row;       /* r^2 as seq_t            */
            return numerator / pos;                   /* floor(r^2 / pos)        */
        }

        cumulative += row_len;              /* move to next row        */
        ++row;                              
    }
}
