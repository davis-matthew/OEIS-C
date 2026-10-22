/*
 * A343568  a(n) = Sum_{d|n} (n/d)^(n/d) * binomial(d+n-1,n).
 * https://oeis.org/A343568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{d|n} (n/d)^(n/d) * binomial(d+n-1,n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used, but safe */
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t m = n / d;
            seq_t pow_term = big_pow((seq_t)m, (seq_t)m);
            seq_t bin_term = binomial(d + n - 1, n);
            sum += pow_term * bin_term;
        }
    }
    return sum;
}
