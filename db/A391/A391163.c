/*
 * A391163  Numbers whose final digit is at most 2.
 * https://oeis.org/A391163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 10 * floor(n/3) + (n mod 3)
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor division and modulus with idx_t (unsigned) */
    seq_t q = (seq_t)(n / 3);     /* block number */
    seq_t r = (seq_t)(n % 3);     /* remainder 0..2 */

    return q * 10 + r;
}
