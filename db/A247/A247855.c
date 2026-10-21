/*
 * A247855  The 10th Hermite Polynomial evaluated at n: H_10(n) = 1024*n^10 -
 *          23040*n^8 + 161280*n^6 - 403200*n^4 + 302400*n^2 - 30240.
 * https://oeis.org/A247855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1024*n^10 - 23040*n^8 + 161280*n^6 - 403200*n^4 +
 *            302400*n^2 - 30240
 * Range:     i8 none, i16 n <= 1, i32 n <= 4, i64 n <= 39, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = -30240 */

seq_t a(idx_t n)
{
    /* convert index to seq_t once to avoid repeated casts */
    seq_t x = (seq_t)n;

    /* compute powers of n safely in seq_t */
    seq_t x2  = x * x;          /* n^2   */
    seq_t x4  = x2 * x2;        /* n^4   */
    seq_t x6  = x4 * x2;        /* n^6   */
    seq_t x8  = x4 * x4;        /* n^8   */
    seq_t x10 = x8 * x2;        /* n^10  */

    /* evaluate the polynomial */
    seq_t result =
          1024 * x10
        - 23040 * x8
        + 161280 * x6
        - 403200 * x4
        + 302400 * x2
        - 30240;

    return result;
}
