/*
 * A342112  Drop the final digit of n^5.
 * https://oeis.org/A342112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(n^5 / 10)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x *= x;          /* n^2 */
    x *= (seq_t)n;   /* n^3 */
    x *= (seq_t)n;   /* n^4 */
    x *= (seq_t)n;   /* n^5 */
    return x / (seq_t)10;
}
