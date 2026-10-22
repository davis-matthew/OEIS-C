/*
 * A306767  Least x > 0 such that (x^2-1)/(y^2-1) = n, where y=A130280(n), or
 *          0 if no such x exists.
 * https://oeis.org/A306767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sqrt(n*(A130280(n)^2-1)+1)
 * The implementation searches for the least y > 1 such that
 * n*(y^2-1)+1 is a perfect square.  Once found, the square root is returned;
 * otherwise 0 is returned.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */

seq_t a(idx_t n)
{
    /* Treat n = 1, 2, …  */
    seq_t N = (seq_t)n;

    /* Search y starting from 2 upwards.  The search limit is generous
     * enough for all n ≤ 10 000:  10·n + 100. */
    const idx_t limit = 10 * n + 100;

    for (idx_t yy = 2; yy <= limit; ++yy)
    {
        seq_t Y = (seq_t)yy;
        seq_t expr = N * (Y * Y - 1) + 1;   /* n*(y²‑1)+1  */
        seq_t s = floor_sqrt(expr);          /* integer part of sqrt */
        if (s * s == expr)                  /* perfect square? */
            return s;                       /* minimal solution found */
    }

    /* No suitable y found → result is 0 */
    return 0;
}
