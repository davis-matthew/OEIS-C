/*
 * A314049  Coordination sequence Gal.3.51.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5, a(2)=11, a(3)=15, a(4)=21, a(5)=26. For n≥6, let
 *            m=n‑5. Then a(n)=26 + 26·⌊m/5⌋ + S(m mod 5), where the partial
 *            sums S(r) are S(0)=0, S(1)=5, S(2)=11, S(3)=15, S(4)=21. (This
 *            corresponds to the 5‑periodic increment pattern 5, 6, 4, 6, 5
 *            for the differences a(n)−a(n‑1) for n≥6.)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* initial terms */
    static const seq_t init[6] = {1, 5, 11, 15, 21, 26};
    if (n <= 5) {
        return init[n];
    }

    /* periodic part */
    const idx_t m = n - 5;
    const idx_t cycles = m / 5;
    const idx_t r = m % 5;

    /* prefix sums of one period: 0,5,11,15,21,26 */
    static const seq_t pref[6] = {0, 5, 11, 15, 21, 26};

    seq_t result = (seq_t)26 + (seq_t)26 * (seq_t)cycles + pref[r];
    return result;
}
