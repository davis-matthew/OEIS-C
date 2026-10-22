/*
 * A315196  Coordination sequence Gal.6.202.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * if n < 12: a(n) = [1, 6, 10, 14, 19, 24, 29, 34, 39,
 *            44, 48, 52][n] * otherwise: let k = n / 12, r = n % 12; * a(n) =
 *            58 * k + s[r], where s = [0, 6, 10, 14, 19, 24, 29, 34, 39, 44,
 *            48, 52, 58] (prefix sums of diffs for blocks after the first)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for the first 12 indices. */
    static const seq_t base[12] = {
        1, 6, 10, 14, 19, 24, 29,
        34, 39, 44, 48, 52
    };
    /* Prefix sums of the diffs for a block after the first (k ≥ 1). */
    static const seq_t pref1[13] = {
        0, 6, 10, 14, 19, 24, 29,
        34, 39, 44, 48, 52, 58
    };

    idx_t k = n / 12;
    idx_t r = n % 12;

    if (k == 0) {
        return base[r];
    }
    return (seq_t)58 * (seq_t)k + pref1[r];
}
