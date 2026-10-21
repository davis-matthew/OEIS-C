/*
 * A257846  a(n) = floor(n/6) * (n mod 6).
 * https://oeis.org/A257846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/6) * (n mod 6)
 * Range:     u8 n <= 271, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q = (seq_t)n / 6;        /* floor division by 6 */
    seq_t r = (seq_t)n % 6;        /* remainder modulo 6 */
    return q * r;                  /* product fits in seq_t */
}
