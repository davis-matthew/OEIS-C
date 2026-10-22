/*
 * A350051  Part three of the trisection of A017101: a(n) = 19 + 24*n.
 * https://oeis.org/A350051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19 + 24*n
 * Range:     u8 n <= 9, u16 n <= 2729, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)19 + (seq_t)24 * (seq_t)n;
}
