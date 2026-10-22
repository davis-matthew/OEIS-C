/*
 * A314959  Coordination sequence Gal.6.340.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + c[n mod 12] (for n >= 1) where c = {0, 0, -1, -1,
 *            0, 0, 0, 0, 0, 1, 1, 0}. a(0) = 1
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
        return (seq_t)1;

    static const int8_t c[12] = {0, 0, -1, -1, 0, 0, 0, 0, 0, 1, 1, 0};
    int64_t res = 5 * (int64_t)n + c[n % 12];
    return (seq_t)res;
}
