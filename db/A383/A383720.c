/*
 * A383720  a(0)=3, a(1)=5, a(2)=35; a(n) = 5*a(n-1) + 5*a(n-2) - a(n-3) for n
 *          > 2.
 * https://oeis.org/A383720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1)+5*a(n-2)-a(n-3) a(0) = 3 a(1) = 5 a(2) = 35
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)35;

    seq_t prev3 = (seq_t)3;   /* a(n-3) */
    seq_t prev2 = (seq_t)5;   /* a(n-2) */
    seq_t prev1 = (seq_t)35;  /* a(n-1) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i)
    {
        curr = (seq_t)5 * prev1;          /* 5 * a(n-1) */
        curr += (seq_t)5 * prev2;        /* + 5 * a(n-2) */
        curr -= prev3;                   /* - a(n-3) */

        /* shift for next iteration */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
