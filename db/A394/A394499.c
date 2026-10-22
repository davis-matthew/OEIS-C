/*
 * A394499  Starting at one corner of n X n grid of dots, the maximum number
 *          of edges that can be drawn without lifting the pencil or going
 *          over a previously drawn edge.
 * https://oeis.org/A394499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * ((n - 1)^2 + 1) - (n mod 2), with a(1) = 0
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t nn  = (seq_t)n;
    seq_t nm1 = nn - 1;
    seq_t sq  = nm1 * nm1;
    seq_t val = (seq_t)2 * (sq + 1);
    /* subtract 1 if n is odd */
    if ((n & 1))
        val -= 1;
    return val;
}
