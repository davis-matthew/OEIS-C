/*
 * A379419  a(n) = n + [n*r/t] + [n*s/t], where r = (3/2)^(1/4); s =
 *          (3/2)^(1/2); t = (3/2)^(3/4) and [ ] = floor.
 * https://oeis.org/A379419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/r⌋ + ⌊n/r²⌋, where r = (3/2)^(1/4)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   floor_div_by_r, floor_div_by_r2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_div_by_r.h"
#include "../utils/floor_div_by_r2.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t f1 = floor_div_by_r(n_seq);
    seq_t f2 = floor_div_by_r2(n_seq);
    return n_seq + f1 + f2;
}
