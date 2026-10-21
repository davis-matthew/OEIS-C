/*
 * A264116  Irregular triangle read by rows: T(n,k), n>=1, k>=1, of the
 *          alternating sum of entries 1 through k in the n-th row of A235791;
 *          the first element of column k is in row k(k+1)/2.
 * https://oeis.org/A264116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the (n‑th) entry of the irregular triangle T. T(r,k) =
 *            Σ_{i=1..k} (-1)^{i+1} ⌊ ( r - (i-1)i/2 ) / i ⌋ r is the smallest
 *            integer such that Σ_{j=1}^{r} ⌊ (sqrt(8j+1)-1)/2 ⌋ >= n. This
 *            implementation computes a(n) on demand and works for all n >= 1
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t target = n;     /* 1‑based index within the flattened triangle */
    idx_t row = 1;        /* current row being examined */

    while (1)
    {
        /* length of the current row: largest k with k(k+1)/2 <= row   */
        idx_t len = 0;
        while (((len + 1) * (len + 2)) / 2 <= row)
            ++len;

        /* skip rows that are fully before the desired index */
        if (target > len)
        {
            target -= len;
            ++row;
            continue;
        }

        /* now we are in the row containing the desired element */
        idx_t pos = target;   /* 1‑based position within this row */

        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (idx_t i = 1; i <= pos; ++i)
        {
            idx_t t_prev = (i - 1) * i / 2;           /* (i-1)i/2 */
            seq_t term = (seq_t)(row - t_prev) / i;   /* floor((row - t_prev)/i) */

            if (i & 1)   pos_sum += term;
            else         neg_sum += term;
        }

        return pos_sum - neg_sum;
    }
}
