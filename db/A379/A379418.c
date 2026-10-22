/*
 * A379418  a(n) = n + [n*r/s] + [n*t/s], where r = (3/2)^(1/4); s =
 *          (3/2)^(1/2); t = (3/2)^(3/4) and [ ] = floor.
 * https://oeis.org/A379418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/r) + floor(n*r), r = (3/2)^(1/4)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   floor_n_over_r, floor_n_r, floor_root4_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_n_over_r.h"
#include "../utils/floor_n_r.h"
#include "../utils/floor_root4_idx.h"

/* OEIS sequence A379418 */
seq_t a(idx_t n)
{
    if (n == 0) /* safety, offset is 1 */
        return 0;

    seq_t term      = (seq_t)n;
    seq_t term_low  = (seq_t)floor_n_over_r(n);
    seq_t term_high = (seq_t)floor_n_r(n);

    return term + term_low + term_high;
}
