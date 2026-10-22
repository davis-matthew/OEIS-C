/*
 * A344820  a(n) = Sum_{k=1..n} floor(n/k) * (-n)^(k-1).
 * https://oeis.org/A344820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k) * (-n)^(k-1)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;                 /* (-n)^(k-1) for current k */
    seq_t base   = -(seq_t)n;         /* (-n)  */

    for (idx_t k = 1; k <= n; ++k) {
        result += power * (seq_t)(n / k);   /* floor(n/k) */
        power *= base;                     /* next power of (-n) */
    }
    return result;
}
