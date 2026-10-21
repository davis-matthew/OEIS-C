/*
 * A274323  Number of partitions of n^4 into at most two parts.
 * https://oeis.org/A274323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor(n^4 / 2)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 303, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t s = nn * nn;   /* n^2 */
    s = s * s;           /* (n^2)^2 = n^4 */
    return (s / 2) + 1;  /* floor division plus 1 */
}
