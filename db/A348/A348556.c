/*
 * A348556  Binary expansion contains 4 adjacent 1's.
 * https://oeis.org/A348556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9436 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th integer whose binary representation contains four consecutive 1's. */
#include <stdbool.h>

static inline bool has_four_consecutive_ones(seq_t v)
{
    seq_t w = v;
    while (w >= 0xFu) {            /* at least 4 bits remain to test */
        if ((w & 0xFu) == 0xFu)   /* lower 4 bits are all 1 */
            return true;
        w >>= 1;                 /* shift to test next position */
    }
    return false;
}

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
