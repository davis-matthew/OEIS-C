/*
 * A312655  Coordination sequence Gal.4.62.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 , if n = 0 (14*n + adj) / 3 , otherwise where adj = 0
 *            for n%6 = 0 or 3 adj = –2 for n%6 = 1 adj = –4 for n%6 = 2 adj =
 *            4 for n%6 = 4 adj = 2 for n%6 = 5 (The division is exact for all
 *            n ≥ 1)
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

    idx_t r = n % 6;
    uint64_t num = 14ull * (uint64_t)n;   /* use 64‑bit intermediate to avoid overflow */

    switch (r) {
        case 1: num -= 2; break;
        case 2: num -= 4; break;
        case 4: num += 4; break;
        case 5: num += 2; break;
        default: /* r == 0 or 3 */ break;
    }

    return (seq_t)(num / 3ull);
}
