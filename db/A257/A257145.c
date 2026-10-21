/*
 * A257145  a(n) = 5 * floor( (n+2) / 5) - n with a(0) = 1.
 * https://oeis.org/A257145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5 * floor( (n+2) / 5 ) - n, with a(0) = 1
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) 
        return 1;

    idx_t q = n / 5;       /* quotient of n by 5   */
    idx_t r = n % 5;       /* remainder of n by 5   */
    idx_t k = q + ((r + 2) / 5);  /* floor((n+2)/5) computed without overflow */

    return 5 * (seq_t)k - (seq_t)n;
}
