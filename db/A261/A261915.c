/*
 * A261915  Smallest x such that n can be written as n = x^2 + y^2 + z^2 with
 *          x >= y >= z >= 0, or -1 if no such x exists.
 * https://oeis.org/A261915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = smallest x such that n = x^2 + y^2 + z^2 with x>=y>=z>=0;
 *            return -1 if no such representation exists
 * Range:     i8 n <= 497, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* Handle non‑representable numbers first (classical theorem) */
    /* n is representable as sum of three squares iff it is not of the form
       n = 4^a (8b+7).  The returned function however does a brute‑force
       search for the smallest possible x, which automatically yields -1
       for those n. */

    idx_t max_x = _floor_isqrt_u64(n);        /* maximum possible x */
    for (idx_t x = 0; x <= max_x; ++x)
    {
        seq_t rem = (seq_t)n - (seq_t)x * (seq_t)x;
        if (rem < 0) continue;                /* not needed, safety only */

        idx_t max_y = _floor_isqrt_u64((idx_t)rem);
        if (max_y > x) max_y = x;              /* enforce y <= x */

        for (idx_t y = 0; y <= max_y; ++y)
        {
            seq_t rem2 = rem - (seq_t)y * (seq_t)y;
            if (rem2 < 0) break;               /* safety break */

            idx_t z = _floor_isqrt_u64((idx_t)rem2);
            if ((seq_t)z * (seq_t)z == rem2 && z <= y)
            {
                return (seq_t)x;               /* smallest x found */
            }
        }
    }
    return (seq_t)-1;                          /* no representation */
}
