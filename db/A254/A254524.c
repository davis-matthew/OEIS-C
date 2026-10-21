/*
 * A254524  n is the a(n)-th positive integer having its digitsum.
 * https://oeis.org/A254524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, nice
 * Formula:   a(n) = the number of positive integers m ≤ n with digit_sum(m) =
 *            digit_sum(n)
 * Range:     u8 n <= 311, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    seq_t sum = digit_sum((seq_t)n);
    seq_t count = 0;
    for (idx_t m = 1; m <= n; ++m) {
        if (digit_sum((seq_t)m) == sum)
            ++count;
    }
    return count;
}
