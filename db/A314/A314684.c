/*
 * A314684  Coordination sequence Gal.6.149.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 4*n + c(n) c(n) is the number of indices k (1 ≤ k ≤
 *            n) where the increment is 5. These indices form a periodic
 *            pattern with offset 5: 5,7,11,12,16,18,22,23,27,29,33,34,...
 *            (period 11, 4 indices per block) The count c(n) can be computed
 *            in O(1) using integer arithmetic
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   count_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_5.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 4;            /* 4*n part */
    seq_t cnt  = (seq_t)count_5(n);      /* extra 5‑increments count */
    return 1 + base + cnt;               /* a(0)=1, a(n)=1+4*n+cnt for n≥0 */
}
