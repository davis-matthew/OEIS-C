/*
 * A338996  Numbers of squares and rectangles of all sizes in
 *          3*n*(n+1)/2-ominoes in form of three-quarters of Aztec diamonds.
 * https://oeis.org/A338996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(7*n+13)/24
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t = nn * (nn + (seq_t)1);
    t = t * (nn + (seq_t)2);
    t = t * (((seq_t)7 * nn) + (seq_t)13);
    t = t / (seq_t)24;
    return t;
}
