/*
 * A308672  a(n) = Sum_{d|n} d^(d^3).
 * https://oeis.org/A308672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(d^3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor i */
            idx_t d = i;
            idx_t d3 = d * d * d;
            sum += pow_uint((uint64_t)d, d3);

            /* paired divisor */
            idx_t j = n / i;
            if (j != d) {
                idx_t d2 = j;
                idx_t d23 = d2 * d2 * d2;
                sum += pow_uint((uint64_t)d2, d23);
            }
        }
    }
    return sum;
}
