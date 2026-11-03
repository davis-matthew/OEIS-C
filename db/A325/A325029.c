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
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   ipow_i32, ipow_idx, isqrt, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"
#include "../utils/isqrt.h"
#include "../utils/num_divisors.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Compute a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* n = 0 is outside the OEIS offset */

    seq_t sum = 0;
    idx_t sqrt_n = isqrt(n);

    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d != 0) continue;

        /* Process divisor d */
        {
            seq_t sigma = sigma_of(d);
            uint64_t tau = num_divisors(d);
            seq_t pod = ipow_idx((seq_t)d, (idx_t)(tau / 2));
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
            uint64_t tau = num_divisors(d2);
            seq_t pod = ipow_idx((seq_t)d2, (idx_t)(tau / 2));
            if (tau % 2 == 1) {
                seq_t sq = floor_sqrt((seq_t)d2);
                pod = pod * sq;
            }
            sum += sigma * pod;
        }
    }

    return sum;
}
