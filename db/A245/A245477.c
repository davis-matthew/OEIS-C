/*
 * A245477  Period 6: repeat [1, 1, 1, 1, 1, 2].
 * https://oeis.org/A245477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*7/6) - floor(n*7/6)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Period 6: return 2 when n mod 6 == 5, otherwise 1 */
    idx_t m = n % 6;
    return (m == 5) ? 2 : 1;
}
