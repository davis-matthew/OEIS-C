/*
 * A258841  a(n) = 9*n^2 - 237*n + 1927.
 * https://oeis.org/A258841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 9*n^2 - 237*n + 1927
 * Range:     u8 none, u16 n <= 98, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 9 * nn * nn - 237 * nn + 1927;
}
