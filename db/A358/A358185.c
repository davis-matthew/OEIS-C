/*
 * A358185  Coefficients of x^n/n! in the expansion of (1 - x)*log(1 - x).
 * https://oeis.org/A358185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=0, a(1)=-1, a(n)=(n-2)! for n>=2
 * Range:     i8 n <= 7, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* a(0) = 0 */
    if (n == 1) return (seq_t)-1;      /* a(1) = -1 */

    seq_t result = 1;
    idx_t max = n - 2;                 /* n >= 2 here, so max is safe */
    for (idx_t i = 2; i <= max; ++i) {
        result *= (seq_t)i;            /* multiply by i */
    }
    return result;
}
