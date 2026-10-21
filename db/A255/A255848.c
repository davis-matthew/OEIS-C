/*
 * A255848  a(n) = 2*n^2 + 18.
 * https://oeis.org/A255848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 18
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t result = tn * tn; /* n^2 */
    result *= 2;            /* 2*n^2 */
    result += 18;           /* +18 */
    return result;
}
