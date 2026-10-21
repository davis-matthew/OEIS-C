/*
 * A247850  The 5th Hermite Polynomial evaluated at n: H_5(n) = 32*n^5 -
 *          160*n^3 + 120*n.
 * https://oeis.org/A247850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 32*n^5 - 160*n^3 + 120*n
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 36, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the 5th Hermite polynomial evaluated at n. */
seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    seq_t t2 = t * t;           /* n^2 */
    seq_t t3 = t2 * t;          /* n^3 */
    seq_t t5 = t3 * t2;          /* n^5 */
    return 32 * t5 - 160 * t3 + 120 * t;
}
