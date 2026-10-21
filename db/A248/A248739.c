/*
 * A248739  a(n) = 29*n + ceiling(n/29).
 * https://oeis.org/A248739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*n + ceil(n/29)
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t mult = (seq_t)29 * n;
    idx_t q = n / 29;
    idx_t r = n % 29;
    seq_t ceil_div = (seq_t)(q + (r != 0));
    return mult + ceil_div;
}
