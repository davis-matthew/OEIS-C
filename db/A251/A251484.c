/*
 * A251484  Number of (n+1) X (1+1) 0..3 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements less than the minimum of its
 *          antidiagonal elements.
 * https://oeis.org/A251484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 20*a(n-2) Initial values: a(1) = 236, a(2) =
 *            3456
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* not part of the defined range */
    if (n == 1)
        return (seq_t)236;
    if (n == 2)
        return (seq_t)3456;

    seq_t prev2 = (seq_t)236; /* a(1) */
    seq_t prev1 = (seq_t)3456; /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t cur = prev1 * (seq_t)16;
        cur -= prev2 * (seq_t)20;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
