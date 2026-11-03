/*
 * A326746  a(n) = (sum of digits of n) mod (sum of digits of n+1).
 * https://oeis.org/A326746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digit_sum(n) mod digit_sum(n+1)
 * Range:     u8 n <= 489, u16 n <= 19999, u32 n <= 19999, u64 n <= 19999,
 *            u128 n <= 19999, big any n
 * Validated: standard, pass, 108 terms through n = 9989 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t sum_n   = digit_sum((seq_t)n);
    seq_t sum_np1 = digit_sum((seq_t)(n + 1));
    return sum_n % sum_np1;
}
