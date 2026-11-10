/*
 * A342711  Partial sums of A000267.
 * https://oeis.org/A342711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A000267(k), where A000267(k) =
 *            floor(sqrt(4k+1)); computed in O(1) by blocks r^2..r^2+2r,
 *            4r^2+3r+1 per full block
 * Range:     u8 n <= 33, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 56 (2026-11-10)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n)
{
    uint64_t R = isqrt_u64((uint64_t)n);
    seq_t Rs = (seq_t)R;
    seq_t total = 0;
    if (R > 0) {
        seq_t s2 = (Rs - 1) * Rs * (2 * Rs - 1) / 6;
        seq_t s1 = Rs * (Rs - 1) / 2;
        total = 4 * s2 + 3 * s1 + Rs;
    }
    seq_t m = (seq_t)n - Rs * Rs + 1;
    seq_t c1 = (m < Rs) ? m : Rs;
    seq_t rest = m - c1;
    total += c1 * 2 * Rs + rest * (2 * Rs + 1);
    return total;
}
