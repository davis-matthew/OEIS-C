/*
 * A266336  G.f. = b(2)*b(6)/(x^6-x^4+x^2-2*x+1), where b(k) = (1-x^k)/(1-x).
 * https://oeis.org/A266336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. = b(2)*b(6)/(x^6-x^4+x^2-2*x+1), b(k) = (1-x^k)/(1-x)
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 50, u64 n <= 105, u128 n <= 215,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prev[6] = {0, 0, 0, 0, 0, 0};   /* prev[0] = a[n-1], prev[1] = a[n-2], ... */
    for (idx_t i = 0; i <= n; ++i)
    {
        /* compute c_i = (i+1) - (i-1) - (i-5) + (i-7) for i ≥ 8, 0 otherwise   */
        seq_t c = (seq_t)i + 1;
        if (i >= 2) c -= ((seq_t)i - 1);
        if (i >= 6) c -= ((seq_t)i - 5);
        if (i >= 8) c += ((seq_t)i - 7);

        seq_t a_val = 2 * prev[0] - prev[1] + prev[3] - prev[5] + c;

        if (i == n) return a_val;

        /* shift the window for the next iteration */
        prev[5] = prev[4];
        prev[4] = prev[3];
        prev[3] = prev[2];
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = a_val;
    }
    return 0;   /* unreachable, but keeps compiler quiet */
}
