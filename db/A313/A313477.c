/*
 * A313477  Coordination sequence Gal.5.54.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9*n - ( base[(n-1) mod 9] + floor((n-1)/9) )) / 2 with
 *            base = {-1,-2,-1,0,1,2,3,2,1}. a(0)=1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* Use signed 64‑bit arithmetic for the intermediate constants. */
    static const int64_t base[9] = {-1, -2, -1, 0, 1, 2, 3, 2, 1};

    idx_t k = n - 1;                      /* offset from the first non‑trivial term */
    int64_t offset = (int64_t)(k / 9);
    int64_t diff    = base[k % 9] + offset;

    int64_t numer = (int64_t)9 * (int64_t)n - diff; /* always even */
    return (seq_t)(numer / 2);
}
