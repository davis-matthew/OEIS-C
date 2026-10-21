/*
 * A257849  a(n) = floor(n/9) * (n mod 9).
 * https://oeis.org/A257849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(n/9) * (n mod 9)
 * Range:     u8 n <= 294, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n / 9) * ((seq_t)n % 9);
}
