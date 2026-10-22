/*
 * A315654  Coordination sequence Gal.3.49.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n > 0: a(n) = 28 * floor(n/5) + c[n mod 5] where c
 *            = {0, 6, 12, 16, 22}
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t offsets[5] = {0, 6, 12, 16, 22};

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t q = n / 5;
    idx_t r = n % 5;
    seq_t base = (seq_t)q * 28;
    return base + offsets[r];
}
