/*
 * A262141  The first of nine consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of ten
 *          consecutive positive integers.
 * https://oeis.org/A262141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=181, a(2)=7021, a(3)=266761,
 *            a(n)=39*a(n-1)-39*a(n-2)+a(n-3) for n>3 (OEIS A262141)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset starts at 1; guard against n==0. */
    if (n == 0)
        return 0;

    /* Base cases. */
    if (n == 1) return (seq_t)181;
    if (n == 2) return (seq_t)7021;
    if (n == 3) return (seq_t)266761;

    /* Iteratively compute the recurrence. */
    seq_t prev3 = (seq_t)181;   /* a(n-3) */
    seq_t prev2 = (seq_t)7021;  /* a(n-2) */
    seq_t prev1 = (seq_t)266761;/* a(n-1) */
    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t cur = (seq_t)39 * prev1 - (seq_t)39 * prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
