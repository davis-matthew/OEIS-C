/*
 * A248159  Expansion of (1 - 2*x^2)/(1 + x)^4. Third column of Riordan
 *          triangle A248156.
 * https://oeis.org/A248159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n*(n+1)*(6 + 7*n - n*n)/6
 * Range:     i8 n <= 9, i16 n <= 34, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t un = (seq_t)n;                 /* convert index to seq_t */
    seq_t n2   = un * un;                /* n^2 */
    seq_t coeff = (seq_t)6 + (seq_t)7 * un - n2;   /* 6 + 7n - n^2 */
    seq_t prod  = (un + 1) * coeff;     /* (n+1)*(6 + 7n - n^2) */
    seq_t res   = prod / (seq_t)6;      /* divide by 6 */
    if (n & 1)                          /* apply (-1)^n sign */
        res = -res;
    return res;
}
