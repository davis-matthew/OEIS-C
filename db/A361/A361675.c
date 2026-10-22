/*
 * A361675  Constant term in the expansion of (1 + x*y*z + w*y*z + w*x*z +
 *          w*x*y + 1/(w*x*y*z))^n.
 * https://oeis.org/A361675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/7⌋} (4k)!/(k!^4) * binomial(7k,4k) *
 *            binomial(n,7k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 58,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 7U;
    for (idx_t k = 0ULL; k <= maxk; ++k)
    {
        /* (4k)!/(k!^4) = C(4k,k) * C(3k,k) * C(2k,k) */
        seq_t term = binom(4 * k, k);
        term *= binom(3 * k, k);
        term *= binom(2 * k, k);

        term *= binom(7 * k, 4 * k);   /* C(7k,4k)   */
        term *= binom(n, 7 * k);       /* C(n,7k)    */

        sum += term;
    }
    return sum;
}
