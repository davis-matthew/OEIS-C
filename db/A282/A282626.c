/*
 * A282626  Exponential expansion of the real root y = y(x) of y^3 - 3*x*y -
 *          1.
 * https://oeis.org/A282626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ∏_{j=1}^{n-1} (n + 1 – 3*j), a(0)=1, a(1)=1
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;                /* empty product */

    seq_t result = (seq_t)1;
    for (idx_t j = 1; j < n; ++j) {
        seq_t term = (seq_t)n + (seq_t)1 - (seq_t)3 * j;
        if (term == (seq_t)0)          /* zero factor --> whole product is zero */
            return (seq_t)0;
        result *= term;
    }
    return result;
}
