/*
 * A384241  a(n) = Product_{k=0..n-1} (3*n-4*k).
 * https://oeis.org/A384241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ∏_{k=0}^{n-1} (3*n-4*k)
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 9, i64 n <= 16, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 1, and a(n) = 0 for n > 0 and n divisible by 4. */
    if (n == 0U)
        return (seq_t)1;
    if (n % 4U == 0U)
        return (seq_t)0;

    seq_t res = (seq_t)1;

    for (idx_t k = 0U; k < n; ++k) {
        seq_t term = ( (seq_t)3 * (seq_t)n ) - ( (seq_t)4 * (seq_t)k );
        res *= term;
    }

    return res;
}
