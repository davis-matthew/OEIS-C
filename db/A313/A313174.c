/*
 * A313174  Coordination sequence Gal.6.478.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 when n = 0. * For n > 0: * if n is even: a(n) = 9*n/2 *
 *            if n ≡ 1 (mod 4): a(n) = (19*n - 3)/4 * if n ≡ 3 (mod 4): a(n) =
 *            (19*n + 3)/4
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if ((n & 1) == 0) {
        return (seq_t)n * 9 / 2;
    } else {
        if ((n & 3) == 1) {
            return ((seq_t)19 * n - 3) / 4;
        } else { /* n % 4 == 3 */
            return ((seq_t)19 * n + 3) / 4;
        }
    }
}
