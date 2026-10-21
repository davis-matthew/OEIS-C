/*
 * A289216  a(n) = n! * Laguerre(n,-10).
 * https://oeis.org/A289216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Laguerre(n,-10) Recurrence: a(0)=1, a(1)=11, for
 *            n>=1: a(n+1) = (2n+11) * a(n) - n*n * a(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    seq_t a_nm1 = (seq_t)1;   /* a(0) */
    seq_t a_n   = (seq_t)11;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        idx_t m = i - 1;                      /* nth index in recurrence */
        seq_t coeff = (seq_t)(2 * m + 11);    /* (2n+11) */
        seq_t term1 = coeff * a_n;            /* coeff * a(n) */

        seq_t sq = (seq_t)(m * m);            /* n^2 */
        seq_t term2 = sq * a_nm1;             /* n^2 * a(n-1) */

        seq_t a_np1 = term1 - term2;          /* a(n+1) */

        a_nm1 = a_n;
        a_n   = a_np1;
    }
    return a_n;
}
