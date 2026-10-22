/*
 * A329999  Beatty sequence for sqrt(x-1), where 1/sqrt(x-1) + 1/sqrt(x+1) =
 *          1.
 * https://oeis.org/A329999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * sqrt(x-1)), where x satisfies
 *            1/sqrt(x-1)+1/sqrt(x+1)=1. Using the relation alpha = sqrt(x-1)
 *            = 1 + δ with δ≈0.783010..., the sequence can be written as a(n)
 *            = n + floor(n * δ). The value δ is the unique real root of δ^4 +
 *            2δ^3 + 2δ^2 - 2δ - 1 = 0, lying in (0,1). The floor of nδ...
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   floor_n_delta
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_n_delta.h"

seq_t a(idx_t n)
{
    /* a(n) = n + floor(n * δ)  with δ ≈ 0.783010…  */
    idx_t k = floor_n_delta(n);
    return (seq_t)n + (seq_t)k;
}
