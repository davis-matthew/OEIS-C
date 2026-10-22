/*
 * A304421  Number of nX2 0..1 arrays with every element unequal to 1, 2, 3, 4
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) + 2*a(n-3) Initial values: a(1)=1,
 *            a(2)=7, a(3)=25
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For the OEIS offset, a(1) is the first term.
       The behaviour for n==0 is undefined; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)7;
    if (n == 3)
        return (seq_t)25;

    /* Iterate from 4 through n keeping the last three computed terms. */
    seq_t prev3 = (seq_t)1;   /* a(i-3) */
    seq_t prev2 = (seq_t)7;   /* a(i-2) */
    seq_t prev1 = (seq_t)25;  /* a(i-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t curr = (seq_t)3 * prev1 + (seq_t)3 * prev2 + (seq_t)2 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
