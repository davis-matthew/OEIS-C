/*
 * A391677  a(n) = Sum_{k=0..n} (k+1) * 2^k * 3^(n-k) * binomial(k,n-k)^2.
 * https://oeis.org/A391677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+1) * 2^k * 3^(n-k) * binomial(k,n-k)^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    seq_t sum   = 0;
    seq_t pow2  = 1;                     /* 2^k */
    seq_t pow3  = pow_uint(3, n);         /* 3^(n-k) when k=0 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin = binomial(k, n - k);  /* returns 0 if n-k > k */
        if (bin) {
            seq_t term = (seq_t)(k + 1) * pow2 * pow3 * bin * bin;
            sum += term;
        }
        /* advance to next k */
        pow2 *= 2;
        /* pow3 is always divisible by 3 */
        if (k < n) {
            pow3 /= 3;
        }
    }
    return sum;
}
