/*
 * A361701  Constant term in the expansion of (1 + x^4 + y^4 + z^4 +
 *          1/(x*y*z))^n.
 * https://oeis.org/A361701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/7⌋} binomial(3k,k) * binomial(2k,k) *
 *            binomial(7k,3k) * binomial(n,7k) (OEIS A361701)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The largest k that contributes is floor(n/7). */
    idx_t kmax = n / 7;
    seq_t sum = 0;

    for (idx_t k = 0; k <= kmax; ++k)
    {
        /* Compute the four binomial factors one by one. */
        seq_t t = binomial(3 * k, k);          /* (3k)!/(k!^2)   */
        t *= binomial(2 * k, k);              /* (2k)!/(k!^2) */
        t *= binomial(7 * k, 3 * k);          /* multinomial    */
        t *= binomial(n, 7 * k);              /* choose 7k out of n */

        sum += t;
    }

    return sum;
}
