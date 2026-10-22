/*
 * A326812  Expansion of Sum_{k>=1} (2^k - 1) * x^(2^k - 1) / (1 - x^(2^k -
 *          1)).
 * https://oeis.org/A326812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d = 2^j - 1, j>=1} d
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (int j = 1; ; ++j) {
        uint64_t d;
        if (j < 64) {
            d = (1ULL << j) - 1ULL;
        } else if (j == 64) {
            d = UINT64_MAX;
        } else {
            break;
        }
        if (d > n) break;
        if (n % d == 0) {
            sum += (seq_t)d;
        }
    }
    return sum;
}
