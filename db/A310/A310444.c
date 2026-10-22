/*
 * A310444  Coordination sequence Gal.6.340.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + f(n) where f(n) is a periodic offset of period 12,
 *            given by the array below for n >= 1. a(0) = 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count each vertex at distance n from a fixed vertex in the Galebach tiling Gal.6.340.1 */

#include <stdint.h>

static const int s_offset[12] = {
    -1,  0,  0, -1,  0,  0,  0,  1,  0,  0,  1,  0
};

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* base = 5*n */
    seq_t base = (seq_t)n * (seq_t)5;

    /* periodic offset for n >= 1 */
    idx_t idx = (n - (idx_t)1) % (idx_t)12;
    int off = s_offset[idx];

    if (off >= 0) {
        base += (seq_t)off;
    } else {
        base -= (seq_t)(-off);
    }

    return base;
}
