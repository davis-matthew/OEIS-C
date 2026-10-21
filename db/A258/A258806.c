/*
 * A258806  a(n) = n^7 + 1.
 * https://oeis.org/A258806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 565, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t r = x * x;   /* n^2   */
    r *= x;            /* n^3   */
    r *= x;            /* n^4   */
    r *= x;            /* n^5   */
    r *= x;            /* n^6   */
    r *= x;            /* n^7   */
    r += 1;            /* n^7 + 1 */
    return r;
}
