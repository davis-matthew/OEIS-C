/*
 * A246425  In the Collatz 3x+1 problem: start at an odd number 2n+1 and find
 *          the next odd number 2m+1 in the trajectory; then a(n) = m-n.
 * https://oeis.org/A246425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice, look
 * Formula:   a(n) = ((6*n+4)/2^A087230(n) - (2*n+1))/2 * For an odd start
 *            2n+1, start on the next odd after one Collatz step. * The
 *            function applies 3x+1 to the odd number 2n+1 and repeatedly *
 *            divides by 2 until the result is odd again. * Let that odd
 *            result be 2m+1; then a(n) = m - n
 * Range:     i8 n <= 20, i16 n <= 7179, i32 n <= 9491, i64 n <= 9491,
 *            i128 n <= 9491, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* cast index to sequence type */
    seq_t x = 2 * N + 1;         /* starting odd number 2n+1   */
    seq_t y = 3 * x + 1;         /* 3x+1                       */

    /* divide by 2 until odd. The while condition tests parity using
       bitwise AND.  The shift operator >> keeps the division exact. */
    while ((y & 1) == 0)
        y >>= 1;                 /* remove even factors of 2 */

    seq_t m = (y - 1) >> 1;      /* the next odd integer is 2m+1 */
    return m - N;                /* a(n) = m - n                 */
}
