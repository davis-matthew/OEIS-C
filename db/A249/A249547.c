/*
 * A249547  a(n) = (10*n^2+8*n-1+(-1)^n)/8.
 * https://oeis.org/A249547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n^2+8*n-1+(-1)^n)/8
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;                /* convert index to the sequence type   */
    seq_t term = (seq_t)10 * N * N + (seq_t)8 * N; /* 10*n^2 + 8*n            */
    if (N & (seq_t)1)                  /* if n is odd subtract 2               */
        term -= (seq_t)2;
    return term / (seq_t)8;            /* exact division by 8                  */
}
