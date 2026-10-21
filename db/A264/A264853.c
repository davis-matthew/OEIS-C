/*
 * A264853  a(n) = n*(n + 1)*(5*n^2 + 5*n - 4)/12.
 * https://oeis.org/A264853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(5*n^2 + 5*n - 4)/12
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sn  = (seq_t)n;               /* cast index to seq_t */
    seq_t n1  = sn + 1;                 /* n + 1  */
    seq_t tri = sn * n1;                /* n*(n+1) */

    seq_t n_sq = sn * sn;
    seq_t val   = (n_sq * 5) + (sn * 5) - 4;   /* 5*n^2 + 5*n - 4 */

    seq_t prod  = tri * val;            /* n*(n+1)*(5*n^2 + 5*n - 4) */
    return prod / 12;
}
