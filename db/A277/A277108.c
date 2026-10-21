/*
 * A277108  a(n) = 4*n*(n+5).
 * https://oeis.org/A277108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(n+5)
 * Range:     u8 n <= 5, u16 n <= 125, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x = x * (x + (seq_t)5); /* x*(x+5) */
    x *= (seq_t)4;          /* *4 */
    return x;
}
