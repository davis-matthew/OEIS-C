/*
 * A359863  a(n) = Sum_{d|n} d^(n/d) * (n/d)^d.
 * https://oeis.org/A359863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n/d) * (n/d)^d
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 35, u64 n <= 89, u128 n <= 206,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"  /* provides big_pow(seq_t, seq_t) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n should be at least 1, but guard anyway */

    seq_t sum = 0;

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t m = n / d;          /* the co‑divisor */

            seq_t term1 = big_pow((seq_t)d, (seq_t)m);  /* d^(n/d)   */
            seq_t term2 = big_pow((seq_t)m, (seq_t)d);  /* (n/d)^d   */
            sum += term1 * term2; /* contribution of d */

            if (d != m) {
                /* contribution of the paired divisor m; product is identical */
                sum += term1 * term2; 
            }
        }
    }
    return sum;
}
