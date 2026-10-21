/*
 * A244725  a(n) = 5*n^3.
 * https://oeis.org/A244725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^3
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 950, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t = t * t;          /* n^2 */
    t = t * (seq_t)n;   /* n^3 */
    return t * 5;       /* 5*n^3 */
}
