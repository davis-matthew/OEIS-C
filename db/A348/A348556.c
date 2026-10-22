/*
 * A348556  Binary expansion contains 4 adjacent 1's.
 * https://oeis.org/A348556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_four_consecutive_ones
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th integer whose binary representation contains four consecutive 1's. */
#include <stdbool.h>
#include "../utils/has_four_consecutive_ones.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for the offset, but safe guard */

    idx_t count = 0;
    seq_t x = 0;

    while (1) {
        if (has_four_consecutive_ones(x)) {
            ++count;
            if (count == n)
                return x;
        }
        ++x;
    }
}
