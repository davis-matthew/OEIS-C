/*
 * A279169  a(n) = floor( 4*n^2/5 ).
 * https://oeis.org/A279169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( 4*n^2/5 )
 * Range:     u8 n <= 7, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t *= t;
    t *= 4;
    return t / 5;
}
