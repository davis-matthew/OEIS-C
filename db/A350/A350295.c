/*
 * A350295  2nd subdiagonal of the triangle A350292.
 * https://oeis.org/A350295
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)/2 for n ≥ 5; a(3) = 6; a(4) = 8
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)8;
    return (seq_t)n * ((seq_t)n - 1) / 2;
}
