/*
 * A397669  a(n) = binomial(3*n-4,n).
 * https://oeis.org/A397669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = binomial(3*n-4, n)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases for negative upper arguments */
    if (n == 0) return 1;
    if (n == 1) return (seq_t)-1;

    /* Upper argument of the binomial is guaranteed positive for n >= 2 */
    seq_t upper = (seq_t)3 * n - 4;

    /* Compute binomial(upper, n) multiplicatively */
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i) {
        result = result * (upper - (seq_t)(i - 1));
        result /= (seq_t)i;
    }
    return result;
}
