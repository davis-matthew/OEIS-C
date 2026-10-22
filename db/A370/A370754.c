/*
 * A370754  a(n) = 2 + n^2*floor((n+3)/2) + floor(3*n/2).
 * https://oeis.org/A370754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + n^2*floor((n+3)/2) + floor(3*n/2)
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 2046, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the result type to avoid mixing types */
    seq_t N = (seq_t)n;
    /* floor((n+3)/2) */
    seq_t half = (N + (seq_t)3) / (seq_t)2;
    /* floor(3*n/2) */
    seq_t third = (seq_t)3 * N / (seq_t)2;
    /* n^2 * half */
    seq_t val = N * N;
    val *= half;
    /* compute the final result */
    return (seq_t)2 + val + third;
}
