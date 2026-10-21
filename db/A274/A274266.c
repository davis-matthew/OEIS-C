/*
 * A274266  Expansion of e.g.f. (1 + x)^3*log(1 + x).
 * https://oeis.org/A274266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * 6 * (n-4)! (n ≥ 4) a(1) = 1, a(2) = 5, a(3) = 11
 *            (offset 1)
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 15, i64 n <= 23, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)11;

    /* For n ≥ 4 */
    seq_t sign = (n & 1) ? (seq_t)-1 : (seq_t)1;   /* (-1)^n */
    seq_t fact = (seq_t)1;                         /* (n-4)! */

    idx_t m = n - 4;                               /* m = n-4 ≥ 0 */
    /* Compute the factorial iteratively */
    for (idx_t i = 2; i <= m; ++i)
        fact *= (seq_t)i;

    /* 6 * (n-4)! with the appropriate sign */
    seq_t result = sign * (seq_t)6 * fact;
    return result;
}
