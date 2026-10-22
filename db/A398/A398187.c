/*
 * A398187  Triangle read by rows: T(n, k) = Sum_{j=0..n-k} ((n - k)! / j!) *
 *          n^j.
 * https://oeis.org/A398187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   For the n‑th term read row‑wise, a(n) = T(r,k) with r = row
 *            index, k = column index. T(r,k) = Σ_{j=0}^{r-k} ((r-k)! / j!) *
 *            r^j
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r that contains index n.
       The cumulative number of terms up to the end of row r
       is (r+1)(r+2)/2.  Find the smallest r such that this value
       exceeds n. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Starting index of row r */
    idx_t start = r * (r + 1) / 2;
    /* Column index within the row */
    idx_t k = n - start;
    /* The upper limit m = r - k of the inner sum */
    idx_t m = r - k;

    /* Compute T(r,k) = Σ_{j=0}^{m} (m! / j!) * r^j */
    seq_t term = 1;          /* term for j=0 : m! */
    for (idx_t i = 1; i <= m; ++i)
        term *= (seq_t)i;    /* term = m! */

    seq_t sum = term;
    for (idx_t j = 1; j <= m; ++j) {
        term = (term * (seq_t)r) / (seq_t)j;  /* term = term * r / j   */
        sum += term;
    }

    return sum;
}
