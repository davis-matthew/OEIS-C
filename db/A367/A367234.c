/*
 * A367234  G.f. satisfies A(x) = 1 + x*A(x)^2 / (1 - x*A(x))^4.
 * https://oeis.org/A367234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n + k + 1, k) * binomial(n + 3k -
 *            1, n - k) / (n + k + 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin1 = binomial(n + k + 1, k);
        seq_t bin2 = binomial(n + 3 * k - 1, n - k);
        uint64_t d = (uint64_t)(n + k + 1);

        /* Cancel factors of d from bin2 */
        seq_t g = gcd((seq_t)d, bin2);
        if (g != (seq_t)0)
        {
            bin2 /= g;
            d /= (uint64_t)g;
        }

        /* Cancel remaining factors of d from bin1 */
        g = gcd((seq_t)d, bin1);
        if (g != (seq_t)0)
        {
            bin1 /= g;
            d /= (uint64_t)g;
        }

        /* After canceling, d should be 1. Compute term */
        seq_t term = bin1 * bin2;
        if (d != 1)
            term /= (seq_t)d;

        sum += term;
    }
    return sum;
}
