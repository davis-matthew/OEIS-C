/*
 * A247146  As a binary numeral, the bit 2^(m-1) of a(n) is 1 iff m is a
 *          proper divisor of n.
 * https://oeis.org/A247146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, d < n} 2^(d-1)
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 1 the sum is empty, giving a(1) = 0. */
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }

    seq_t sum = 0;
    /* Iterate over divisors up to sqrt(n). */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* i is a divisor. */
            if (i < n) {
                sum += pow2(i - 1);
            }
            /* Complementary divisor n / i. */
            idx_t j = n / i;
            if (j != i && j < n) {
                sum += pow2(j - 1);
            }
        }
    }
    return sum;
}
