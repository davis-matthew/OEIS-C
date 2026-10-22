/*
 * A339916  The sum of 2^((d-1)/2) over all divisors of 2n+1.
 * https://oeis.org/A339916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d | (2n+1)} 2^((d-1)/2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    uint64_t m = 2ULL * (uint64_t)n + 1ULL;

    for (uint64_t d = 1; d <= m; d += 2) {
        if (m % d == 0) {
            idx_t e = (idx_t)((d - 1) / 2);
            sum += pow2(e);
        }
    }
    return sum;
}
