/*
 * A339481  a(n) = Sum_{d|n} d^(n-d) * binomial(d+n/d-2, d-1).
 * https://oeis.org/A339481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n-d) * binomial(d+n/d-2, d-1)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 27, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d  = i;
            idx_t nd = n / i;

            /* term for divisor d */
            seq_t powd   = big_pow((seq_t)d, (seq_t)(n - d));
            seq_t binod  = binomial((idx_t)(d + nd - 2), (idx_t)(d - 1));
            sum += powd * binod;

            /* term for complementary divisor nd (if distinct) */
            if (d != nd) {
                seq_t pownd  = big_pow((seq_t)nd, (seq_t)(n - nd));
                seq_t binond = binomial((idx_t)(nd + d - 2), (idx_t)(nd - 1));
                sum += pownd * binond;
            }
        }
    }
    return sum;
}
