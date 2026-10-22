/*
 * A376014  a(n) = Sum_{d|n} d^d * binomial(n/d,d).
 * https://oeis.org/A376014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^d * binomial(n/d, d)
 * Range:     u8 n <= 14, u16 n <= 34, u32 n <= 95, u64 n <= 251,
 *            u128 n <= 719, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where the sum runs over all positive divisors d of n. */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* iterate divisors up to sqrt(n).  For d > sqrt(n) the binomial
       term binomial(n/d, d) is 0 (since n/d < d), so they need not be
       examined.  */
    for (idx_t d = 1; (__int128)d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            seq_t pow_dd = big_pow((seq_t)d, (seq_t)d);
            seq_t term = pow_dd * binomial(n / d, d);
            result += term;
        }
    }
    return result;
}
