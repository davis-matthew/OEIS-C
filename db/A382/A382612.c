/*
 * A382612  a(n) = n^3 * (n^2 - n + 1).
 * https://oeis.org/A382612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^3 * (n^2 - n + 1)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t t2 = tn * tn;          /* n^2 */
    seq_t t3 = t2 * tn;          /* n^3 */
    seq_t t5 = t2 - tn + (seq_t)1;/* n^2 - n + 1 */
    return t3 * t5;              /* n^3 * (n^2 - n + 1) */
}
