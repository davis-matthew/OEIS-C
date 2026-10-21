/*
 * A283366  Number of ways to write 2*n + 1 as x^2 + y^2 + 2*z^2 with x,y,z
 *          integers such that 2*x + y + z is a square or a power of two.
 * https://oeis.org/A283366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_power_of_two_uint, _is_square_uint, _isqrt_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A283366: Number of representations of 2*n+1 as
 *  x^2 + y^2 + 2*z^2 with integer x,y,z such that
 *  2*x + y + z is a square or a power of two.
 *
 *  This implementation enumerates all integer triples (x,y,z)
 *  with |x|,|y| <= sqrt(2*n+1) and |z| <= sqrt((2*n+1)/2).
 *  For each triple that satisfies the equation and the
 *  additional condition on 2*x+y+z it increments a counter.
 *  The counter is of type seq_t, so it works for normal
 *  integral types as well as arbitrary‑precision types.
 *  Complexity: O(n^(3/2)) per call (about 16 million
 *  iterations for n = 10000), which fits easily into the
 *  time limits for the required ranges.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_is_power_of_two_uint.h"
#include "../utils/_is_square_uint.h"
#include "../utils/_isqrt_uint.h"

/* Main sequence function.  Builds the count from scratch for
 * each call.  No caching is used to keep the implementation
 * simple and portable to all seq_t types (including big integers). */
seq_t a(idx_t n)
{
    /* Limit of the equation: 2*n + 1 (odd). */
    unsigned limit = (unsigned)(2 * (unsigned)n + 1);
    /* Bounds for the loops. */
    unsigned max_xy = (unsigned)_isqrt_uint(limit);
    unsigned max_z   = (unsigned)_isqrt_uint(limit / 2);

    seq_t count = 0; /* Zero of type seq_t */

    for (int sx = -(int)max_xy; sx <= (int)max_xy; ++sx) {
        unsigned sx2 = (unsigned)sx * (unsigned)sx;
        if (sx2 > limit) continue;
        for (int sy = -(int)max_xy; sy <= (int)max_xy; ++sy) {
            unsigned sy2 = (unsigned)sy * (unsigned)sy;
            unsigned sum_xy = sx2 + sy2;
            if (sum_xy > limit) continue;
            for (int sz = -(int)max_z; sz <= (int)max_z; ++sz) {
                unsigned sz2 = (unsigned)sz * (unsigned)sz;
                unsigned total = sum_xy + 2 * sz2;
                if (total != limit) continue;  /* equality check */

                /* Condition on 2*x + y + z */
                int cond = 2 * sx + sy + sz;
                unsigned ucond = (unsigned)(cond < 0 ? -cond : cond); /* sign not needed */

                bool ok;
                if (cond < 0) {
                    ok = false;                     /* negative can't be square or positive power of two */
                } else {
                    ok = _is_square_uint(ucond)   || _is_power_of_two_uint(ucond);
                }
                if (ok) {
                    count += 1;
                }
            }
        }
    }
    return count;
}
