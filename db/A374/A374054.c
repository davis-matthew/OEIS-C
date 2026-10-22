/*
 * A374054  a(n) = max_{i=0..n} S_3(i) + S_3(n-i) where S_3(x) = A053735(x) is
 *          the base-3 digit sum of x.
 * https://oeis.org/A374054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 2*floor(log_3(n+1)) + S_3(n+1) - 1, where S_3(x) is the
 *            base‑3 digit sum
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _digit_sum_ternary, _floor_log3_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_digit_sum_ternary.h"
#include "../utils/_floor_log3_uint64.h"

seq_t a(idx_t n)
{
    idx_t np1 = n + 1;
    unsigned log3 = _floor_log3_uint64(np1);
    seq_t sum = _digit_sum_ternary(np1);
    seq_t result = (seq_t)2 * log3 + sum - 1;
    return result;
}
