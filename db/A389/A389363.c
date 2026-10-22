/*
 * A389363  a(n) is the number of isosceles trapezoids in a triangular grid of
 *          side length n.
 * https://oeis.org/A389363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 - 1)*(3*n^2 + 4*n - 3)/16)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 199, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;                      /* n^2 */
    seq_t t1  = n2 - (seq_t)1;              /* n^2 - 1 */
    seq_t t2  = (seq_t)3 * n2 + (seq_t)4 * N - (seq_t)3; /* 3*n^2 + 4*n - 3 */
    seq_t prod = t1 * t2;                   /* (n^2 - 1)*(3*n^2 + 4*n - 3) */
    return prod / (seq_t)16;                /* floor division by 16 */
}
