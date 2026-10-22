/*
 * A359112  a(n) = Sum_{d|n} (n/d) * d^(n-d).
 * https://oeis.org/A359112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d) * d^(n-d)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    seq_t sum = 0;

    /* compute floor(sqrt(n)) without floating point */
    idx_t limit = 0;
    while ((limit + 1) <= n / (limit + 1))
        ++limit;

    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d1;

            seq_t term1 = (seq_t)(n / d1) *
                big_pow((seq_t)d1, (seq_t)(n - d1));
            sum += term1;

            if (d1 != d2) {
                seq_t term2 = (seq_t)(n / d2) *
                    big_pow((seq_t)d2, (seq_t)(n - d2));
                sum += term2;
            }
        }
    }
    return sum;
}
