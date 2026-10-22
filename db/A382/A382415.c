/*
 * A382415  Numbers with at least one zero in their base-5 representation.
 * https://oeis.org/A382415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_zero_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with at least one zero in base-5 representation */

#include <stdbool.h>
#include "../utils/has_zero_base5.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 0;
    for (;;)
    {
        if (has_zero_base5(x))
        {
            ++count;
            if (count == n) return x;
        }
        ++x;
    }
}
