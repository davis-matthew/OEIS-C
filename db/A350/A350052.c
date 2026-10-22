/*
 * A350052  Third part of the trisection of A017077: a(n) = 17 + 24*n.
 * https://oeis.org/A350052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17 + 24*n
 * Range:     u8 n <= 9, u16 n <= 2729, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)17 + (seq_t)24 * (seq_t)n;
}
