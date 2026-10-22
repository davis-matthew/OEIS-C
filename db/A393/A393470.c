/*
 * A393470  Triangle read by rows: T(n, k) is the number of ordered pairs (i,
 *          j) of integers with 0 <= i, j < n such that floor((i*j) / n) = k.
 * https://oeis.org/A393470
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k) where m is the smallest integer ≥2 such that
 *            m(m-1)/2 ≥ n-1, and k = (n-1) − (m-1)(m-2)/2 − 1. T(m,k) counts
 *            ordered pairs (i,j), 0 ≤ i,j < m, with floor((i*j)/m) = k. The
 *            sequence is the triangle read by rows, starting at n = 2
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* undefined for n < 2, return 0 */

    /* Position in the concatenated sequence (starting at 1) */
    idx_t p = n - 1;

    /* Find the row number m such that m(m-1)/2 ≥ p. */
    idx_t m = 2;
    while ((m * (m - 1)) / 2 < p)
        ++m;

    /* offset k within the row (0‑based). */
    idx_t prev_total = ((m - 1) * (m - 2)) / 2;
    idx_t k = p - prev_total - 1;

    /* Count pairs (i,j) with floor(i*j/m) == k. */
    idx_t count = 0;
    for (idx_t i = 0; i < m; ++i)
        for (idx_t j = 0; j < m; ++j) {
            idx_t prod = i * j;
            if (prod >= k * m && prod < (k + 1) * m)
                ++count;
        }

    return (seq_t)count;
}
