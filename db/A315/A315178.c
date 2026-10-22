/*
 * A315178  Coordination sequence Gal.4.62.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 2 + 2*(floor(n/6) + floor((n-1)/6)) for n ≥ 1, a(0)
 *            = 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t n_seq   = (seq_t)n;
    seq_t count0  = (seq_t)(n / 6);
    seq_t count1  = (seq_t)((n - 1) / 6);
    return 4 * n_seq + 2 + 2 * (count0 + count1);
}
