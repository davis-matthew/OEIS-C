/*
 * A304442  Number of partitions of n in which the sequence of the sum of the
 *          same summands is constant.
 * https://oeis.org/A304442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} binomial(d(n/d), d) for n > 0, with a(0) = 1
 * Range:     u8 n <= 119, u16 n <= 1439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 1;

    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0)
            continue;

        idx_t nd = n / d;                     // n / d
        idx_t dcnt = (idx_t)divisor_count_u64(nd); /* number of divisors of nd */

        if (d <= dcnt) {
            sum += binomial(dcnt, d);
        }
    }
    return sum;
}
