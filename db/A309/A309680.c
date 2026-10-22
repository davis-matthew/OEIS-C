/*
 * A309680  The smallest nonsquare nonzero integer that is a quadratic residue
 *          modulo n, or 0 if no such integer exists.
 * https://oeis.org/A309680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the smallest non‑zero integer that is a quadratic residue
   modulo n but is not a perfect square.  Returns 0 if no such integer
   exists. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t min_val = (seq_t)0;
    bool found = false;

    for (idx_t y = 1; y < n; ++y) {
        seq_t r = (seq_t)y * (seq_t)y % (seq_t)n;
        if (r == (seq_t)0)          /* skip residue zero */
            continue;

        seq_t s = floor_sqrt(r);
        if (s * s == r)             /* skip perfect square integers */
            continue;

        if (!found || r < min_val) {
            min_val = r;
            found = true;
            if (min_val == (seq_t)2)   /* minimal possible value */
                break;
        }
    }

    return found ? min_val : (seq_t)0;
}
