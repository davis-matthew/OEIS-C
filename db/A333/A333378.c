/*
 * A333378  a(n) = F(n) * (-1)^(n*(n-1)/2) where F(n) = A000045(n) Fibonacci
 *          numbers.
 * https://oeis.org/A333378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = A000045(n) * (-1)^(n*(n-1)/2) (Fibonacci times sign
 *            pattern)
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the sequence A333378, defined for n >= 0. */
seq_t a(idx_t n)
{
    /* sign pattern: + for n%4==0 or 1, - for n%4==2 or 3 */
    seq_t sign = ((n & 3) == 0 || (n & 3) == 1) ? 1 : -1;

    /* Fibonacci numbers: iterative computation */
    if (n == 0)          /* F(0) == 0, sign * 0 = 0 */
        return 0;

    seq_t f0 = 0; /* F(0) */
    seq_t f1 = 1; /* F(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t f2 = f0 + f1; /* F(i) = F(i-1) + F(i-2) */
        f0 = f1;
        f1 = f2;
    }

    /* f1 now holds F(n) */
    return sign * f1;
}
