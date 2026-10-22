/*
 * A366975  Expansion of Sum_{k >=1} x^(3*k)/(1-x^k)^(k+1).
 * https://oeis.org/A366975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} binomial(d + n/d - 3, d) for n > 1. a(1) = 0,
 *            a(2) = 0. The function uses the provided binomial routine from
 *            ../utils/binomial.h. It runs in O(√n) time
 * Range:     u8 n <= 27, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special cases: 1 and 2 give zero directly. */
    if (n <= 2)
        return 0;

    seq_t sum = 0;

    /* Iterate over the divisors of n up to sqrt(n). */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d1 = i;
            seq_t term = binomial(d1 + n / d1 - 3, d1);
            sum += term;

            /* second divisor d2 = n / i, avoid double counting. */
            idx_t d2 = n / i;
            if (d2 != d1) {
                term = binomial(d2 + n / d2 - 3, d2);
                sum += term;
            }
        }
    }
    return sum;
}
