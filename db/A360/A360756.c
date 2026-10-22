/*
 * A360756  Expansion of Sum_{k>0} (x * (1 + 2 * x^k))^k.
 * https://oeis.org/A360756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^(n/d-1) * binomial(d, n/d-1)
 * Range:     u8 n <= 23, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1 – for n < 1 the function is not defined,
       but we return 0 for safety. */
    if (n < 1) return 0;

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* First divisor d */
            idx_t k1 = n / d;                     /* n/d */
            sum += pow2(k1 - 1) * binomial(d, k1 - 1);

            /* Complementary divisor d2 = n/d, avoid double counting for squares */
            idx_t d2 = n / d;
            if (d2 != d) {
                idx_t k2 = n / d2; /* equals d */
                sum += pow2(k2 - 1) * binomial(d2, k2 - 1);
            }
        }
    }

    return sum;
}
