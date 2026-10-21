/*
 * A255165  a(n) = Sum_{k=2..n} floor(log(n)/log(k)), n >= 1.
 * https://oeis.org/A255165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊log2(n)⌋} (⌊n^(1/i)⌋ - 1)
 * Range:     u8 n <= 232, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_root_idx, pow_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_root_idx.h"
#include "../utils/pow_le.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* maximum exponent i such that 2^i ≤ n */
    idx_t max_i = 0;
    if (n > 0)
        max_i = 63 - __builtin_clzll(n);

    seq_t sum = 0;
    for (idx_t i = 1; i <= max_i; ++i) {
        idx_t root = floor_root_idx(n, i);
        sum += (seq_t)(root - 1);
    }
    return sum;
}
