/*
 * A370845  a(n) = (n-2)^n*n!.
 * https://oeis.org/A370845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n-2)^n*n!
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 11, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small n explicitly to avoid unsigned underflow */
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)0;

    /* compute (n-2)^n */
    seq_t base = (seq_t)(n - 2);    /* n >= 3 → base >= 1 */
    seq_t pow_res = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow_res *= base;

    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    return pow_res * fact;
}
