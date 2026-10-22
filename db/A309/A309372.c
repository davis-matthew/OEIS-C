/*
 * A309372  a(n) = n^2 - n^3 + n^4.
 * https://oeis.org/A309372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - n^3 + n^4
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;          /* convert index to seq_t */
    seq_t t2 = t * t;            /* n^2 */
    seq_t t3 = t2 * t;           /* n^3 */
    seq_t t4 = t3 * t;           /* n^4 */
    return t4 - t3 + t2;         /* n^4 - n^3 + n^2 */
}
