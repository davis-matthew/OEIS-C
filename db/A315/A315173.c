/*
 * A315173  Coordination sequence Gal.5.99.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n >= 1: a(n) = a(n-1) + 5 if n == 1 a(n) = a(n-1)
 *            + 4 + 2*I otherwise, where I = 1 if (n-2) mod 8 >= 6, and I = 0
 *            otherwise. This gives the periodic step pattern
 *            5,4,4,4,4,4,4,6,6,4,4,4,4,4,4,6,6,... and matches the OEIS data
 *            for A315173
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t a_val = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t diff;
        if (i == 1) {
            diff = (seq_t)5;
        } else {
            idx_t r = (i - 2) % 8;          /* (i-2) is safe because i>=2 */
            diff = (seq_t)4 + ((r >= 6) ? (seq_t)2 : (seq_t)0);
        }
        a_val += diff;
    }
    return a_val;
}
