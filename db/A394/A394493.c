/*
 * A394493  Expansion of e.g.f. (1/x) * Series_Reversion( x*(1 - 3*x)^(2/3) ).
 * https://oeis.org/A394493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/(n+1)) * Π_{j=0}^{n-1} (5*n-1-3*j)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t denom = (seq_t)(n + 1);
    seq_t result = 1;

    for (idx_t j = 0; j < n; ++j) {
        /* term = 5*n - 1 - 3*j  */
        seq_t term = (seq_t)(5 * n - 1 - 3 * j);

        /* cancel any factor of denom that divides this term */
        seq_t g = gcd(term, denom);
        if (g > 1) {
            term /= g;
            denom /= g;
        }

        result *= term;
    }

    /* denom should be 1 now, else the product was not divisible;
       it is guaranteed by the definition of the sequence. */
    return result;
}
