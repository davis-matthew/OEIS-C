/*
 * A391164  Numbers whose final digit is at most 5.
 * https://oeis.org/A391164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 10 * floor(n/6) + (n mod 6)
 * Range:     u8 n <= 155, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 6;
    idx_t r = n % 6;
    return (seq_t)q * 10 + (seq_t)r;
}
