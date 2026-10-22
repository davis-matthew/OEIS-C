/*
 * A310379  Coordination sequence Gal.6.321.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 4 For n ≥ 2: a(n) = 4*n + 2 * count{ k | 2 ≤ k
 *            ≤ n, k ≡ 2,5,6,9 (mod 10) }
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
    if (n >= 2) count += (seq_t)((n - 2) / 10 + 1);
    if (n >= 5) count += (seq_t)((n - 5) / 10 + 1);
    if (n >= 6) count += (seq_t)((n - 6) / 10 + 1);
    if (n >= 9) count += (seq_t)((n - 9) / 10 + 1);

    return (seq_t)4 * n + (seq_t)2 * count;
}
