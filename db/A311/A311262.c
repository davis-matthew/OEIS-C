/*
 * A311262  Coordination sequence Gal.6.216.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4. For n >= 2, a(n) = 4*n + cnt, where cnt is the
 *            number of k with 2 <= k <= n and k ≡ 5 or 7 (mod 11)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    seq_t count = 0;
    if (n >= 5) count += (seq_t)((n - 5) / 11 + 1);
    if (n >= 7) count += (seq_t)((n - 7) / 11 + 1);
    return (seq_t)4 * n + count;
}
