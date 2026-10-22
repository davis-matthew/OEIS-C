/*
 * A386851  a(n) = floor(5*n^2/6).
 * https://oeis.org/A386851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*n^2/6)
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n2 = (seq_t)n * (seq_t)n;   /* n^2 */
    seq_t prod = n2 * 5;              /* 5*n^2 */
    return prod / 6;                  /* floor division by 6 */
}
