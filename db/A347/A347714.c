/*
 * A347714  Number of compositions (ordered partitions) of n into at most 2
 *          cubes.
 * https://oeis.org/A347714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A347714: Number of compositions of n into at most two positive cubes. */
/* Implementation: Count the single-cube case and all ordered pairs of positive cubes whose sum is n. */

#include <stdint.h>
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    if (n == 0)           // empty composition for n = 0
        return 1;

    seq_t ans = 0;

    idx_t maxk = int_cube_root(n);
    for (idx_t k = 1; k <= maxk; ++k) {
        idx_t cube = k * k * k;
        idx_t rem  = n - cube;
        if (rem == 0)
            continue;                  // both parts must be positive
        idx_t r = int_cube_root(rem);
        if ((seq_t)r * r * r == rem)  // rem is a cube
            ++ans;
    }

    /* add the single‑cube composition (if any) */
    idx_t r0 = int_cube_root(n);
    if ((seq_t)r0 * r0 * r0 == n)
        ++ans;

    return ans;
}
