/*
 * A264120  Values of k such that A001163(k) is positive.
 * https://oeis.org/A264120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((-1)^n + 4*n - 7)/2
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    if (n & 1)   /* n is odd: a(n) = 2n - 4 */
        return (seq_t)(2 * (seq_t)n - 4);
    else         /* n is even: a(n) = 2n - 3 */
        return (seq_t)(2 * (seq_t)n - 3);
}
