/*
 * A284551  Triangular array read by rows, demonstrating that the difference
 *          between a pentagonal number (left edge of triangle) and a square
 *          (right edge) is a triangular number.
 * https://oeis.org/A284551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (m(3m-1) - (k-1)k)/2, where m = the smallest integer with
 *            m(m+1)/2 ≥ n, and k = n - (m-1)m/2
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but harmless */

    /* Find m such that T_{m-1} < n ≤ T_m, T_m = m(m+1)/2. */
    idx_t m = 1;
    idx_t t = 1;                     /* T_m for current m */
    while (t < n) {
        ++m;
        t += m;                      /* t = m(m+1)/2 */
    }
    idx_t t_prev = t - m;            /* T_{m-1} */
    idx_t k = n - t_prev;            /* 1 ≤ k ≤ m */

    /* Compute the formula using seq_t arithmetic. */
    seq_t m_s = (seq_t)m;
    seq_t p_m = m_s * ((m_s * 3) - 1) / 2;          /* pentagonal number P(m) */
    seq_t kk  = (seq_t)k;
    seq_t sub = (kk - 1) * kk / 2;                  /* (k-1)k/2 */
    return p_m - sub;
}
