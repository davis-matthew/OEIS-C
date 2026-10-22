/*
 * A348954  a(n) = Sum_{d|n, d < sqrt(n)} (-1)^(n/d) * d.
 * https://oeis.org/A348954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, d < sqrt(n)} (-1)^(n/d) * d
 * Range:     i8 n <= 863, i16 n <= 9529, i32 n <= 9529, i64 n <= 9529,
 *            i128 n <= 9529, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Iterate over all divisors d of n that satisfy d < sqrt(n).
       The condition d < n/d is equivalent to d < sqrt(n) and
       avoids overflow for large n. */
    for (idx_t d = 1; d < n / d; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;          /* exponent n/d   */
            if (e & 1)                /* exponent odd?  */
                sum -= (seq_t)d;      /* sign = -1      */
            else
                sum += (seq_t)d;      /* sign = +1      */
        }
    }
    return sum;
}
