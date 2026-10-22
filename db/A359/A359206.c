/*
 * A359206  a(n) = Sum_{d|n} 4^(n-d).
 * https://oeis.org/A359206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 4^(n-d) (OEIS A359206)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* the sequence is defined only for n ≥ 1 */
    seq_t sum = 0;
    idx_t i, d, d2, e;
    for (i = 1; i <= n / i; ++i) { /* iterate over divisors without overflow */
        if (n % i == 0) {
            /* divisor d = i */
            d = i;
            e = n - d;
            sum += pow_uint(4, e);

            /* paired divisor d2 = n / i */
            d2 = n / i;
            if (d2 != d) {
                e = n - d2;
                sum += pow_uint(4, e);
            }
        }
    }
    return sum;
}
