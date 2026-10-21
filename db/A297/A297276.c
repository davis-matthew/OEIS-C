/*
 * A297276  Numbers whose base-12 digits have greater down-variation than
 *          up-variation; see Comments.
 * https://oeis.org/A297276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   greater_down_variation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297276: search for numbers whose base‑12 down‑variation
   is greater than its up‑variation. */

#include <stdbool.h>
#include "../utils/greater_down_variation.h"

seq_t a(idx_t n)
{
    seq_t candidate = 1;
    idx_t count = 0;

    while (1)
    {
        if (greater_down_variation(candidate))
        {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
