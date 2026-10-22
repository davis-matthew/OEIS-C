/*
 * A314096  Coordination sequence Gal.4.73.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1: a(n)=a(n-1) + d_{n-1}, where the differences
 *            d_k are d_0 = 4, d_1 = 6, d_2 = 5, and for k≥3 d_k =
 *            pattern[(k-3) mod 7] with pattern = {4,5,6,5,5,6,5}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t val = 1;
    static const seq_t pattern[7] = {4, 5, 6, 5, 5, 6, 5};
    for (idx_t i = 0; i < n; ++i) {
        seq_t diff;
        if (i == 0)          diff = 4;
        else if (i == 1)     diff = 6;
        else if (i == 2)     diff = 5;
        else                 diff = pattern[(i - 3) % 7];
        val += diff;
    }
    return val;
}
