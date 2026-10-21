/*
 * A241158  Numbers in which the two leading (most significant) digits are
 *          distinct.
 * https://oeis.org/A241158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose two most significant decimal digits
 *            are distinct (or the integer has fewer than two digits). The
 *            sequence starts with a(1)=0
 * Range:     u8 n <= 227, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_distinct_leading_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_distinct_leading_digits.h"

seq_t a(idx_t n)
{
    /* a(1) corresponds to 0, so handle it immediately */
    if (n <= 1)
        return 0;

    idx_t  count = 1;   /* we already have 0 as the first term */
    seq_t  candidate = 0;

    while (count < n)
    {
        ++candidate;
        if (has_distinct_leading_digits(candidate))
            ++count;
    }

    return candidate;
}
