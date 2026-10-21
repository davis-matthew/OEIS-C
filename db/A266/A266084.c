/*
 * A266084  Expansion of (5 - x - x^2 - x^3 - x^4 + 4*x^5)/( x^6 - x^5 - x +
 *          1).
 * https://oeis.org/A266084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 + 5*floor(n/5) - n mod 5
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 5;
    idx_t r = n % 5;
    return (seq_t)5 + (seq_t)5 * (seq_t)q - (seq_t)r;
}
