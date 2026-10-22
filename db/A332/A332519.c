/*
 * A332519  a(n) = 4*(n^2 + n - 2).
 * https://oeis.org/A332519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 4*(n^2 + n - 2)
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;                 /* cast index to seq_t first */
    seq_t sq = tn * tn;                  /* tn^2 */
    seq_t val = 4 * (sq + tn - 2);       /* 4*(n^2 + n - 2) */
    return val;
}
