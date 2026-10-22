/*
 * A312892  Coordination sequence Gal.5.50.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + #{k ≤ n | k ≡ 2 or 9 (mod 10)}, a(0) = 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    /* base contribution */
    seq_t base = (seq_t)4 * n;

    /* count of indices k with k ≡ 2 or 9 (mod 10), k ≥ 2 and k ≤ n */
    seq_t extra = 0;
    if (n >= 2) {
        idx_t m = n - 2;
        idx_t full_cycles = m / 10;               /* each full decade contributes 2 */
        extra = (seq_t)2 * full_cycles;

        idx_t start = 2 + full_cycles * 10;        /* first index of the partial cycle */
        for (idx_t j = start; j <= n; ++j) {
            if (j % 10 == 2 || j % 10 == 9) ++extra;
        }
    }

    return base + extra;
}
