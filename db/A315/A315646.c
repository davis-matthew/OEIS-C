/*
 * A315646  Coordination sequence Gal.5.290.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 6 For n ≥ 2: a(n) = a(n-1) + (n % 9 == 3 || n %
 *            9 == 5 || n % 9 == 7 ? 4 : 6) This can be written closed‑form
 *            as: a(n) = 6 * n - 2 * k where k = |{ i ∈ [2, n] : i mod 9 ∈
 *            {3,5,7} }|
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* Count indices i in [2, n] with i % 9 in {3,5,7}. */
    idx_t k = 0;
    const idx_t residues[3] = {3, 5, 7};
    for (int r = 0; r < 3; ++r) {
        if (n >= residues[r]) {
            k += (n - residues[r]) / 9 + 1;
        }
    }

    /* Compute result using seq_t arithmetic. */
    seq_t result = (seq_t)6 * (seq_t)n - (seq_t)2 * (seq_t)k;
    return result;
}
