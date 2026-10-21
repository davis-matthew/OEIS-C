/*
 * A267489  a(n) = n^2 - 4*floor(n^2/6).
 * https://oeis.org/A267489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - 4*floor(n^2/6)
 * Range:     u8 n <= 15, u16 n <= 307, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;        /* n^2 */
    seq_t q  = sq / (seq_t)6;              /* floor(n^2/6) */
    return sq - (seq_t)4 * q;              /* n^2 - 4*floor(n^2/6) */
}
