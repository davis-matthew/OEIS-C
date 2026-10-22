/*
 * A359203  a(n) = Sum_{d|n} (n/d) * 3^(n-d).
 * https://oeis.org/A359203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} (n/d) * 3^(n-d)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return sum;  // safety, though n >= 1 in OEIS context

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d1 = i;
            seq_t factor1 = (seq_t)(n / d1);          /* n/d1 = i */
            seq_t pow1   = pow3(n - d1);
            sum += factor1 * pow1;

            /* divisor d2 = n/i, distinct from d1 */
            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t factor2 = (seq_t)(n / d2);     /* n/d2 = i */
                seq_t pow2   = pow3(n - d2);
                sum += factor2 * pow2;
            }
        }
    }
    return sum;
}
