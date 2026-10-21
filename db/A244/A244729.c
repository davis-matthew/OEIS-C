/*
 * A244729  a(n) = 10*n^3.
 * https://oeis.org/A244729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n^3
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 754, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t = t * t * t;    /* compute n^3 */
    t = t * 10;       /* multiply by 10 */
    return t;
}
