/*
 * A296911  Partial sums of A296910.
 * https://oeis.org/A296911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 2*n^2 + 2*n - (-1)^n for n >= 1
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)2 * nn * (nn + (seq_t)1);

    /* subtract 1 for even n, add 1 for odd n */
    if ((n & 1) == 0)
        val -= (seq_t)1;
    else
        val += (seq_t)1;

    return val;
}
