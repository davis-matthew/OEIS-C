/*
 * A271894  Expansion of (1+x-3*x^2) / ( 1-2*x-4*x^2+6*x^3 ).
 * https://oeis.org/A271894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 6*a(n-3) with initial values:
 *            a(0)=1, a(1)=3, a(2)=7
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 7;

    /* iterate using the recurrence */
    seq_t a0 = 1;   /* a(n-3)  */
    seq_t a1 = 3;   /* a(n-2)  */
    seq_t a2 = 7;   /* a(n-1)  */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i)
    {
        ai = 2 * a2 + 4 * a1;   /* 2*a(n-1) + 4*a(n-2) */
        ai -= 6 * a0;           /* subtract 6*a(n-3) */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
