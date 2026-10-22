/*
 * A310469  Coordination sequence Gal.5.141.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0 * a(n) = 5*n + 1 if n mod 5 = 4 * a(n) = 5*n
 *            - 1 if n mod 5 = 1 * a(n) = 5*n otherwise
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    idx_t r = n % 5;
    if (r == 4) {
        return (seq_t)n * 5 + 1;
    }
    if (r == 1) {
        return (seq_t)n * 5 - 1;
    }
    return (seq_t)n * 5;
}
