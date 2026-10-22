/*
 * A344887  a(n) is the least base k >= 2 that the base-k digits of n are
 *          nonincreasing.
 * https://oeis.org/A344887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least base k ≥ 2 such that the base‑k digits of n are
 *            nonincreasing
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_nonincreasing
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_nonincreasing.h"

seq_t a(idx_t n)
{
    /* special handling for n = 0 and 1 */
    if (n <= 1)
        return (seq_t)2;

    /* search from base 2 upward */
    uint64_t n64 = (uint64_t)n;    /* n fits in uint64_t */
    for (uint64_t b = 2; b <= n64; ++b) {
        if (is_nonincreasing(n64, b))
            return (seq_t)b;
    }
    /* fallback (should not be reached, base n always works) */
    return (seq_t)n64;
}
