/*
 * A276026  a(n) = Sum_{k=0..7} (n + k)^2.
 * https://oeis.org/A276026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 + 56*n + 140
 * Range:     u8 n <= 1, u16 n <= 86, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return ((seq_t)8 * nn * nn) + ((seq_t)56 * nn) + (seq_t)140;
}
