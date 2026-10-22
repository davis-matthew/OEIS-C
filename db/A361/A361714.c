/*
 * A361714  a(n) = Sum_{k = 0..n-1}
 *          (-1)^(n+k+1)*binomial(n,k)*binomial(n+k-1,k)^2.
 * https://oeis.org/A361714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k = 0 .. n-1} (-1)^(n+k+1) * C(n,k) * C(n+k-1,k)^2
 *            Offset: 0 The function works for any supported seq_t, including
 *            built‑in integer types and arbitrary‑precision integer classes
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Current binomials:
       c1 = C(n,k),  c2 = C(n+k-1,k) */
    seq_t c1 = 1;
    seq_t c2 = 1;

    seq_t pos = 0;  /* sum of terms with positive sign   */
    seq_t neg = 0;  /* sum of terms with negative sign   */

    for (idx_t k = 0; k < n; ++k)
    {
        /* product = C(n,k) * C(n+k-1,k)^2 */
        seq_t prod = c1 * c2 * c2;

        /* sign = (-1)^(n+k+1)  → negative when (n+k+1) odd */
        if (((n + k + 1) & 1ull) != 0)
            neg += prod;
        else
            pos += prod;

        /* Prepare binomials for next k */
        if (k + 1 < n)
        {
            c1 = (c1 * (n - k)) / (k + 1);            /* C(n,k+1) */
            c2 = (c2 * (n + k)) / (k + 1);            /* C(n+k,k+1) */
        }
    }

    /* final result: positive sum minus negative sum   */
    return pos - neg;
}
