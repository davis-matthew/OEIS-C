/*
 * A312979  Coordination sequence Gal.6.247.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((24*n + r(n))/5) for n ≥ 1, with * r(n) periodic of
 *            period 10: * r(1) = -4, r(2) = -3, r(3) = -7, r(4) = -1, r(5) =
 *            0, * r(6) = 1, r(7) = 7, r(8) = 3, r(9) = 4, r(10)= 0. * a(0) =
 *            1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* r array: r[0] corresponds to n = 1 */
    static const int r[10] = { -4, -3, -7, -1, 0, 1, 7, 3, 4, 0 };

    /* Compute (24*n + r(n))/5 exactly */
    idx_t mod = (n - 1) % 10;
    int delta = r[mod];
    int64_t value = (int64_t)24 * (int64_t)n + (int64_t)delta;

    return (seq_t)(value / 5);
}
