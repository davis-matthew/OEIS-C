/*
 * A350534  Sum of the largest parts of the partitions of n into 3 parts whose
 *          largest part is equal to the product of the other two.
 * https://oeis.org/A350534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{⌊n/3⌋} ∑_{i=k}^{⌊(n−k)/2⌋} [n−i−k = i·k] ·
 *            (n−i−k)
 * Range:     u8 n <= 82, u16 n <= 3358, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. sum the largest part of each partition n = x + y + x·y with 1≤x≤y. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t ans = 0;
    for (idx_t k = 1; k <= n / 3; ++k) {
        idx_t rem = n - k;
        idx_t denom = k + 1;
        if (rem % denom != 0) continue;
        idx_t i = rem / denom;
        if (i < k) continue;
        seq_t largest = (seq_t)i * (seq_t)k;
        ans += largest;
    }
    return ans;
}
