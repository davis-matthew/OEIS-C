/*
 * A325029  a(n) = Sum_{d|n} (sigma(d)*pod(d)) where sigma(k) = the sum of the
 *          divisors of k (A000203) and pod(k) = the product of the divisors
 *          of k (A007955).
 * https://oeis.org/A325029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} sigma(d)*pod(d)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   _floor_isqrt_u64, divisor_count_u64, floor_sqrt, seq_pow,
 *            sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"      /* sigma_of */
#include "../utils/divisor_count_u64.h" /* divisor_count_u64 */
#include "../utils/_floor_isqrt_u64.h"  /* _floor_isqrt_u64 */
#include "../utils/floor_sqrt.h"  /* floor_sqrt */
#include "../utils/seq_pow.h"     /* seq_pow */

/* Compute a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* n = 0 is outside the OEIS offset */

    seq_t sum = 0;
    idx_t sqrt_n = _floor_isqrt_u64(n);

    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d != 0) continue;

        /* Process divisor d */
        {
            seq_t sigma = sigma_of(d);
            uint64_t tau = divisor_count_u64(d);
            seq_t pod = seq_pow((seq_t)d, (idx_t)(tau / 2));
            if (tau % 2 == 1) {
                seq_t sq = floor_sqrt((seq_t)d);
                pod = pod * sq;               /* multiplication works for seq_t */
            }
            sum += sigma * pod;
        }

        /* Process complementary divisor n/d if distinct */
        if (d != n / d) {
            idx_t d2 = n / d;
            seq_t sigma = sigma_of(d2);
            uint64_t tau = divisor_count_u64(d2);
            seq_t pod = seq_pow((seq_t)d2, (idx_t)(tau / 2));
            if (tau % 2 == 1) {
                seq_t sq = floor_sqrt((seq_t)d2);
                pod = pod * sq;
            }
            sum += sigma * pod;
        }
    }

    return sum;
}
