/*
 * A393448  a(n) = 3*n^2-2*n+1+floor(n^2/4).
 * https://oeis.org/A393448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 - 2*n + 1 + floor(n^2/4) (offset 0)
 * Range:     u8 n <= 9, u16 n <= 142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;        /* convert index to seq_t */
    seq_t n2  = nn * nn;         /* n^2 */
    seq_t t1  = 3 * n2;          /* 3*n^2 */
    seq_t t2  = 2 * nn;          /* 2*n   */
    seq_t t3  = n2 / 4;          /* floor(n^2/4) */
    return t1 - t2 + 1 + t3;     /* 3*n^2 - 2*n + 1 + floor(n^2/4) */
}
