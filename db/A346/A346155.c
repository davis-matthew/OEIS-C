/*
 * A346155  Partial sums of A007978.
 * https://oeis.org/A346155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^n least_non_divisor(k), where
 *            least_non_divisor(k) is the smallest integer m ≥ 2 such that m ∤
 *            k
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t m = 2;
        while (k % m == 0)
            ++m;
        sum += (seq_t)m;
    }

    return sum;
}
