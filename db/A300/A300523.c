/*
 * A300523  a(n) = (5*n + 5)*(5*n + 6)*(5*n + 7)/6.
 * https://oeis.org/A300523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 5)*(5*n + 6)*(5*n + 7)/6
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 338, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;              /* convert index to seq_t first */
    seq_t t1 = nn * 5 + 5;            /* 5*n + 5   */
    seq_t t2 = nn * 5 + 6;            /* 5*n + 6   */
    seq_t t3 = nn * 5 + 7;            /* 5*n + 7   */
    seq_t num = t1 * t2;              /* intermediate product */
    num = num * t3;                   /* full numerator   */
    return num / 6;                   /* final division   */
}
