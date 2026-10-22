/*
 * A386858  a(n) = floor(5*n^2/8).
 * https://oeis.org/A386858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*n^2/8)
 * Range:     u8 n <= 20, u16 n <= 323, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    tmp = tmp * tmp;   /* n^2  (uses idx_t to avoid overflow for small seq_t) */
    tmp = tmp * 5;     /* 5*n^2 */
    tmp = tmp / 8;     /* floor division by 8 */
    return (seq_t)tmp; /* cast back to the appropriate seq_t type */
}
