/*
 * A386477  a(0) = 1; thereafter a(n) = 2*(6*n^2 - 3*n + 1).
 * https://oeis.org/A386477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n >= 1: a(n) = 2*(6*n^2 - 3*n + 1)
 * Range:     u8 n <= 4, u16 n <= 74, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t nm = (seq_t)n;
    seq_t t = nm * nm;           /* n^2                     */
    t *= (seq_t)6;               /* 6*n^2                   */
    t -= nm * (seq_t)3;          /* 6*n^2 - 3*n             */
    t += (seq_t)1;               /* 6*n^2 - 3*n + 1         */
    t *= (seq_t)2;               /* 2*(6*n^2 - 3*n + 1)     */
    return t;
}
