/*
 * A263109  n is the a(n)-th positive integer having its digitsum in base-12
 *          representation.
 * https://oeis.org/A263109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4019, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_base12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = #{ i | 1 ≤ i ≤ n  and  sum_{12}(i) = sum_{12}(n) }
   where sum_{12}(x) is the digit sum of x expressed in base‑12. */

#include <stdint.h>
#include "../utils/digit_sum_base12.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not used (offset = 1) */

    int target = digit_sum_base12(n);
    seq_t count = 0;

    for (idx_t i = 1; i <= n; ++i) {
        if (digit_sum_base12(i) == target)
            ++count;
    }
    return count;
}
