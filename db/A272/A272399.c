/*
 * A272399  The intersection of hexagonal numbers (A000384) and centered
 *          9-gonal numbers (A060544).
 * https://oeis.org/A272399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*n^2 - 27*n + 10
 * Range:     u8 n <= 4, u16 n <= 61, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)18 * nn * nn - (seq_t)27 * nn + (seq_t)10;
}
