/*
 * A395633  a(n) = number of triples (x, y, z) such that 2*x^2 + y*z = n,
 *          where x, y, z are positive integers satisfying y < x < z.
 * https://oeis.org/A395633
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: count triples (x, y, z) of positive integers with
   2*x^2 + y*z = n and y < x < z. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* If n is below the OEIS offset, the result is undefined for sequence
       purposes, but returning 0 is harmless. */
    if (n < 11) return 0;

    seq_t count = 0;
    /* Loop over all possible x such that 2*x^2 < n. */
    for (idx_t x = 1; (idx_t)2 * x * x < n; ++x) {
        idx_t M = n - (idx_t)2 * x * x;    /* M = y*z  > 0 */
        /* Enumerate all divisors d of M. */
        for (idx_t d = 1; d * d <= M; ++d) {
            if (M % d != 0) continue;
            idx_t e = M / d;            /* complementary divisor */

            /* Consider (d, e) as (y, z). */
            if (d < x && e > x) ++count;

            /* If d != e, consider (e, d) as (y, z). */
            if (e != d && e < x && d > x) ++count;
        }
    }
    return count;
}
