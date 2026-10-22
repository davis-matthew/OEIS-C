/*
 * A364922  a(n) is the square of the side length of a simplex whose
 *          n-dimensional inner hypervolume is equal to its (n-1)-dimensional
 *          surface hypervolume. As a result, the sequence starts at n=2.
 * https://oeis.org/A364922
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * n^3 * (n + 1)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 215, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;       /* convert index to seq_t */
    seq_t N2 = N * N;         /* n^2 */
    seq_t N3 = N2 * N;        /* n^3 */
    seq_t res = N3 * (seq_t)2;/* 2 * n^3 */
    res *= (N + (seq_t)1);    /* 2 * n^3 * (n + 1) */
    return res;
}
