/*
 * A325030  a(n) = Product_{d|n} (sigma(d)*pod(d)) where sigma(k) = the sum of
 *          the divisors of k (A000203) and pod(k) = the product of the
 *          divisors of k (A007955).
 * https://oeis.org/A325030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} (sigma(d)*pod(d))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 11, u64 n <= 23, u128 n <= 59,
 *            big any n
 * Helpers:   _pod_of, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/_pod_of.h"

seq_t a(idx_t n)
{
    uint64_t un = (uint64_t)n;   /* n is always ≥ 1 */
    seq_t result = 1;

    for (uint64_t d = 1; d * d <= un; ++d) {
        if (un % d == 0) {
            seq_t sigma_d = sigma_of((idx_t)d);
            seq_t pod_d   = _pod_of(d);
            result *= sigma_d * pod_d;

            uint64_t e = un / d;
            if (e != d) {
                seq_t sigma_e = sigma_of((idx_t)e);
                seq_t pod_e   = _pod_of(e);
                result *= sigma_e * pod_e;
            }
        }
    }
    return result;
}
