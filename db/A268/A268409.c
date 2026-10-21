/*
 * A268409  a(n) = 4*a(n - 1) + 2*a(n - 2) for n>1, a(0)=3, a(1)=5.
 * https://oeis.org/A268409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2), a(0)=3, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)5;

    seq_t prev2 = 3;  /* a(n-2) */
    seq_t prev1 = 5;  /* a(n-1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = (seq_t)4 * prev1 + (seq_t)2 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
