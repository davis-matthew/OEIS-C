/*
 * A383997  a(n) = Product_{k=0..n-1} (n-5*k).
 * https://oeis.org/A383997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Π_{k=0}^{n-1} (n − 5·k) (OEIS A383997)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 15, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* n could be large; handle trivial cases first */
    if (n == 0)
        return (seq_t)1;          /* empty product */

    /* If n is a positive multiple of 5, one factor is zero */
    if ((n % 5) == 0)
        return (seq_t)0;

    seq_t result = (seq_t)1;
    for (idx_t k = 0; k < n; ++k) {
        seq_t term = (seq_t)n - (seq_t)5 * (seq_t)k;
        result *= term;
    }
    return result;
}
