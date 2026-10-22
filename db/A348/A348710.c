/*
 * A348710  In the binary expansion of n, decrease the length of each run of
 *          1-bits by one.
 * https://oeis.org/A348710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = binary representation of n with the first 1 of each run
 *            of consecutive 1‑bits removed (equivalently replace each
 *            occurrence of the pattern 01 with 0, treating the
 *            most‑significant bit as preceded by a virtual 0)
 * Range:     u8 n <= 767, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t msb = floor_log2_uint64(n);

    seq_t result   = 0;
    uint8_t prev_bit = 0;        /* virtual leading 0 */

    for (idx_t i = msb; ; --i)
    {
        uint8_t curr_bit = (n >> i) & 1U;   /* current bit of n */

        /* Skip the first 1 of each run of 1s. */
        if (!(curr_bit == 1U && prev_bit == 0U))
            result = (result << 1) | (seq_t)curr_bit;

        prev_bit = curr_bit;

        if (i == 0) break;
    }

    return result;
}
