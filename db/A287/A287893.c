/*
 * A287893  a(n) = floor(n*(n+2)/9).
 * https://oeis.org/A287893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n+2)/9)
 * Range:     u8 n <= 47, u16 n <= 767, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n * ((seq_t)n + 2)) / 9;
}
