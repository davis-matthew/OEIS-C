/*
 * A294039  a(n) = e*Gamma(2*n,1).
 * https://oeis.org/A294039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1)! * sum_{k=0}^{2*n-1} 1/k! Recurrence: a(1) = 2,
 *            a(n+1) = 2*n*(2*n+1)*a(n) + 2*n + 2 (for n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of range, not used in tests */

    seq_t val = 2;                /* a(1) */
    for (idx_t i = 1; i < n; ++i) /* iterate for n-1 steps to reach a(n) */
    {
        seq_t twon = (seq_t)2 * (seq_t)i;        /* 2*i  */
        seq_t factor = twon * (twon + 1);       /* 2*i*(2*i+1) */
        seq_t inc = twon + 2;                   /* 2*i+2   */
        val = factor * val + inc;               /* recurrence */
    }
    return val;
}
