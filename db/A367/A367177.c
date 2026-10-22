/*
 * A367177  Triangle read by rows, T(n, k) = [x^k] hypergeom([1/2, -n, -n],
 *          [1, 1], 4*x).
 * https://oeis.org/A367177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(row, col)^2 * binomial(2*col, col) where (row,
 *            col) are the indices obtained by reading the triangle row by row
 *            (offset 0)
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r and column k for the n-th term (0‑based). */
    idx_t r = 0;
    idx_t cum = 1; /* terms up to row 0 */
    while (cum <= n)
    {
        ++r;
        cum += r + 1;
    }
    idx_t prev_cum = cum - (r + 1);
    idx_t k = n - prev_cum;

    seq_t b1 = binomial_small(r, k);
    seq_t b2 = binomial_small(2 * k, k);
    seq_t res = b1 * b1;
    res = res * b2;
    return res;
}
