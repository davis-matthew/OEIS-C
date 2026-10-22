/*
 * A358333  By concatenating the standard compositions for each part of the
 *          n-th standard composition, we get a sequence of length a(n).
 *          Row-lengths of A357135.
 * https://oeis.org/A358333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of popcount(part) over all parts of the standard *
 *            composition of n (which is derived from the positions of 1’s in
 *            the * reversed binary representation of n)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t popcnt_idx(idx_t v)
{
    seq_t c = 0;
    while (v)
    {
        v &= v - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t result = 0;
    idx_t prev = 0;     /* previous position of a 1‑bit, starts at 0 */
    idx_t pos  = 1;     /* current position index (1‑based) */
    idx_t bits = n;     /* work on a copy of n, shifting it right each loop */

    while (bits)
    {
        if (bits & 1ULL)          /* this position has a 1‑bit */
        {
            idx_t part = pos - prev;          /* difference of positions */
            result += popcnt_idx(part);       /* add popcount of the part */
            prev = pos;                       /* update previous position */
        }
        bits >>= 1;
        ++pos;
    }

    return result;
}
