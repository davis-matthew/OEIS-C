/*
 * A344817  a(n) = Sum_{k=1..n} floor(n/k) * (-2)^(k-1).
 * https://oeis.org/A344817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k) * (-2)^(k-1)
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;              /* running total */
    seq_t p   = 1;              /* (-2)^(k-1) for current k */
    const seq_t neg_two = (seq_t)-2;  /* -2 as seq_t */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = n / k;                /* floor(n/k) */
        sum += (seq_t)t * p;            /* add term to the sum */
        p *= neg_two;                   /* advance power: (-2)^(k) */
    }
    return sum;
}
