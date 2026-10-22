/*
 * A354796  Triangle read by rows. T(n, k) = Gamma(k + n) / k! for n >= 1 and
 *          0 <= k <= n, T(0, 0) = 1.
 * https://oeis.org/A354796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = read triangle T(n, k) by rows, where T(0,0) = 1 and for n
 *            ≥ 1, T(n, k) = Γ(k+n) / k! = ∏_{i=1}^{n-1} (k + i). This
 *            implementation computes the n‑th term by locating the
 *            corresponding row and column and evaluating the product
 *            multiplicatively. The method runs in O(√n) time and uses only
 *            int...
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Special case n = 0 */
    if (n == 0U)
        return (seq_t)1;

    /* Find the row r such that cum(r-1) ≤ n < cum(r) */
    idx_t r     = 0U;        /* current row */
    idx_t cum   = 1U;        /* cumulative count up to row r */

    /* cum contains (r+1)(r+2)/2 for the current r */
    while (cum <= n) {
        ++r;                     /* move to next row */
        cum += r + 1U;           /* add (r+1) items of this row */
    }

    /* cum now equals cum(r) = (r+1)(r+2)/2, and the desired
       index lies in row r.  Compute the column k. */
    idx_t prev_cum = cum - (r + 1U);   /* cum(r-1) */
    idx_t k       = n - prev_cum;     /* column index, 0 ≤ k ≤ r */

    /* Evaluate T(r, k) = ∏_{i=1}^{r-1} (k + i) */
    seq_t val = (seq_t)1;
    for (idx_t i = 1U; i < r; ++i)
        val *= (seq_t)(k + i);

    return val;
}
