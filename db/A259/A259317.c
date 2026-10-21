/*
 * A259317  a(n) = 2*(2*n+1)*A000538(n) - 4*A000330(n)^2.
 * https://oeis.org/A259317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(1+2*n)^2*(-3+n+8*n^2+4*n^3))/45
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Equivalent finite‑difference implementation. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t a_val = 0;
    seq_t diff[6] = {
        (seq_t)2,   /* Δ a0   */
        (seq_t)66,  /* Δ^2 a0 */
        (seq_t)384, /* Δ^3 a0 */
        (seq_t)832, /* Δ^4 a0 */
        (seq_t)768, /* Δ^5 a0 */
        (seq_t)256  /* Δ^6 a0 (constant) */
    };

    for (idx_t i = 0; i < n; ++i)
    {
        seq_t old = diff[0];        /* Δ a at current n     */
        a_val += old;               /* a_{n+1} = a_n + Δ a_n */

        /* Update higher‑order differences.
           Use old values; the loop order ensures each
           new value depends only on old values. */
        for (int k = 0; k < 5; ++k)
            diff[k] += diff[k + 1];
        /* diff[5] remains the constant sixth difference (256). */
    }

    return a_val;
}
