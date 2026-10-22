/*
 * A325644  "Sloping quaternary numbers": write numbers in quaternary under
 *          each other (right-justified), read diagonals in upward direction,
 *          convert to decimal.
 * https://oeis.org/A325644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _digit_at_pos, _len4_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = interpret the digits along the diagonal with
 * index n (where row – column = n) of the table of quaternary
 * representations of integers, read from bottom to top, as a base‑4
 * integer.
 */

#include <stdbool.h>               /* provides 'bool' */
#include "../utils/_digit_at_pos.h"
#include "../utils/_len4_u64.h"

seq_t a(idx_t n)
{
    idx_t pos = 0;
    /* Find the largest pos such that a digit exists at that position */
    while (pos < _len4_u64(n + pos))
        ++pos;
    if (pos == 0)
        return (seq_t)0;  /* only the zero row */
    --pos;                /* now pos is the maximum valid position */

    seq_t result = 0;
    /* Iterate from the bottom of the diagonal (largest pos) to the top (pos 0) */
    while (true) {
        seq_t d = _digit_at_pos(n + pos, pos);
        result = result * 4 + d;      /* build the base‑4 number */
        if (pos == 0)
            break;
        --pos;
    }
    return result;
}
