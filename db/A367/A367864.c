/*
 * A367864  a(n) = Sum_{d|n} d * binomial(n,d).
 * https://oeis.org/A367864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d * binomial(n,d)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* sequence starts at n = 1, but guard */
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            seq_t bin1 = binomial((idx_t)n, (idx_t)d1);
            result += (seq_t)d1 * bin1;
            idx_t d2 = n / d;
            if (d2 != d1) {
                seq_t bin2 = binomial((idx_t)n, (idx_t)d2);
                result += (seq_t)d2 * bin2;
            }
        }
    }
    return result;
}
