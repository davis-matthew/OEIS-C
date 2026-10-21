/*
 * A257850  a(n) = floor(n/10) * (n mod 10).
 * https://oeis.org/A257850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(n/10)*(n mod 10)
 * Range:     u8 n <= 297, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tens  = (seq_t)n / (seq_t)10;
    seq_t digit = (seq_t)n % (seq_t)10;
    return tens * digit;
}
