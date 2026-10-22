/*
 * A308422  a(n) = n^2 if n odd, 3*n^2/4 if n even.
 * https://oeis.org/A308422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^2 if n odd, 3*n^2/4 if n even
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;
    if ((n & 1) == 0)          /* even n */
        return (sq * 3) / 4;   /* 3*n^2/4 */
    else                       /* odd n */
        return sq;             /* n^2 */
}
