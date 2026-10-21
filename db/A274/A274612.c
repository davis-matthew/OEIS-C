/*
 * A274612  Numbers not divisible by 10 with at least one zero but no two
 *          adjacent 0's among its decimal digits.
 * https://oeis.org/A274612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_valid_zero_pattern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search method: iterate integers in increasing order,
   count those that are not divisible by 10, contain at least
   one zero digit, and have no two adjacent zeros.
   Return the n‑th such integer. */
#include <stdbool.h>
#include "../utils/has_valid_zero_pattern.h"

seq_t a(idx_t n)
{
    seq_t candidate = 0;
    idx_t cnt = 0;
    while (cnt < n) {
        candidate++;
        if (has_valid_zero_pattern(candidate))
            cnt++;
    }
    return candidate;
}
