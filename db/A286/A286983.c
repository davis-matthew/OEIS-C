/*
 * A286983  a(n) is the smallest integer that can appear as the n-th term of
 *          two distinct nondecreasing sequences of positive integers that
 *          satisfy the Fibonacci recurrence relation.
 * https://oeis.org/A286983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = F(n)*(1 + F(n-1))
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where F is the Fibonacci sequence with F(0)=0, F(1)=1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* not used for the offset, but safe */

    seq_t f0 = 0;          /* F(n-1) */
    seq_t f1 = 1;          /* F(n)   */

    for (idx_t i = 1; i < n; ++i)
    {
        seq_t f2 = f0 + f1;  /* next Fibonacci number */
        f0 = f1;
        f1 = f2;
    }

    seq_t temp = f0 + 1;   /* 1 + F(n-1) */
    return f1 * temp;       /* F(n) * (1 + F(n-1)) */
}
