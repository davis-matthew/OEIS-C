/*
 * A330035  a(n) = sign(tan(n)).
 * https://oeis.org/A330035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sign(tan(n)) = (-1)^{floor(2n/π)} ; a(0) = 0
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Compute floor(2n / π) using a tight rational bound:
       π is between 103993/33102 (lower) and 104348/33215 (upper).
       Thus floor(2n/π) = floor((2n * 33215) / 104348).     */
    seq_t v = (seq_t)n;
    v = v * 2;                     /* 2n   */
    v = v * (seq_t)33215;
    v = v / (seq_t)104348;

    /* Sign of tan(n) is +1 if the floor is even, -1 if odd.   */
    return (v & 1) ? (seq_t)(-1) : (seq_t)(1);
}
