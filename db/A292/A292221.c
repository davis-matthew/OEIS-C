/*
 * A292221  Expansion of the exponential generating function (1/2)*(1 +
 *          4*x)*(1 - (1 + 4*x)^(-3/2))/x.
 * https://oeis.org/A292221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * binom(2*n+1, n+1) * n! ; a(0)=3
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 14, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;

    seq_t binom = 1;
    idx_t i;
    for (i = 1; i <= n + 1; ++i)
        binom = binom * (((seq_t)n) + (seq_t)i) / (seq_t)i;

    seq_t fact = 1;
    for (i = 2; i <= n; ++i)
        fact = fact * (seq_t)i;

    seq_t val = binom * fact;
    seq_t sign = (n & 1) ? (seq_t)(-1) : (seq_t)1;
    return sign * val;
}
