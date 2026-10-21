/*
 * A291006  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - S - S^2 - S^3 - S^4.
 * https://oeis.org/A291006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 8*a(n-2) + 6*a(n-3) - a(n-4) initial values:
 *            a(0)=1, a(1)=3, a(2)=9, a(3)=27
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fast return for the first four terms */
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 9;
    if (n == 3) return 27;

    /* use the recurrence iteratively */
    seq_t a0 = 1;       /* a(n-4)   */
    seq_t a1 = 3;       /* a(n-3)   */
    seq_t a2 = 9;       /* a(n-2)   */
    seq_t a3 = 27;      /* a(n-1)   */
    seq_t an = 0;       /* current value */

    for (idx_t i = 4; i <= n; ++i) {
        an = 5 * a3;                /* 5*a(n-1)           */
        an -= 8 * a2;               /* -8*a(n-2)          */
        an += 6 * a1;               /* +6*a(n-3)           */
        an -= a0;                   /* -a(n-4)             */

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
