/*
 * A244730  a(n) = 2*n^4.
 * https://oeis.org/A244730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^4
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 215, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;   /* cast index to seq_t */
    seq_t x2 = x * x;      /* n^2 */
    seq_t x4 = x2 * x2;    /* (n^2)^2 = n^4 */
    return x4 * (seq_t)2;  /* 2 * n^4 */
}
