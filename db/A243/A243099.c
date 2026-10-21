/*
 * A243099  A002061 and A000217 interleaved.
 * https://oeis.org/A243099
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n <= 5; a(2k) = k^2-5k+7; a(2k+1) = (k-2)(k-1)/2
 * Range:     u8 n <= 37, u16 n <= 517, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 5) {
        return 0;
    }

    if ((n & 1) == 0) { /* even n */
        seq_t k = (seq_t)(n >> 1);
        uint64_t kk = (uint64_t)k * (uint64_t)k;
        uint64_t val = kk - 5ULL * (uint64_t)k + 7ULL;
        return (seq_t)val;
    } else { /* odd n */
        seq_t k = (seq_t)((n - 1) >> 1);
        uint64_t prod = (uint64_t)(k - 2) * (uint64_t)(k - 1);
        uint64_t val = prod / 2ULL;
        return (seq_t)val;
    }
}
