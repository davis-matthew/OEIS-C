/*
 * A279054  Largest integer m for which binomial(m,n-1) > binomial(m-1,n).
 * https://oeis.org/A279054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( (3*n-3 + sqrt(5*n^2-2*n+1)) / 2 )
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* Compute the discriminant D = 5n^2 - 2n + 1 in seq_t */
    seq_t D = 5 * nn * nn - 2 * nn + 1;

    /* Integer part of the square root of D */
    seq_t f = floor_sqrt(D);

    /* Check whether D is a perfect square */
    bool perfect = (f * f == D);

    /* A = 3n - 3 + f  (all in seq_t) */
    seq_t A = 3 * nn - 3 + f;

    seq_t ans;
    if (perfect) {
        /* If D is a perfect square, the expression is rational */
        ans = (A + 1) / 2;          /* ceil( A/2 ) */
    } else {
        /* If D is not a perfect square, the ceil adds one when A is even */
        ans = (A + 2) / 2;          /* (A/2)+1 if A even, else (A+1)/2 */
    }

    return ans;
}
