/*
 * A315309  Coordination sequence Gal.4.133.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n ≥ 1: a(n) = a(n-1) + d(n) where d(1) = 5, d(n) =
 *            6 if n mod 7 ∈ {0,1,3,5}, d(n) = 4 otherwise
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    seq_t val = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1) {
            val += (seq_t)5;
        } else {
            idx_t m = i % (idx_t)7;
            if (m == 0 || m == 1 || m == 3 || m == 5)
                val += (seq_t)6;
            else
                val += (seq_t)4;
        }
    }
    return val;
}
