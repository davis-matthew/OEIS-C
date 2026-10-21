/*
 * A257845  a(n) = floor(n/5) * (n mod 5).
 * https://oeis.org/A257845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/5) * (n mod 5)
 * Range:     u8 n <= 323, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 5;
    idx_t r = n % 5;
    return (seq_t)q * (seq_t)r;
}
