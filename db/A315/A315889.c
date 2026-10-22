/*
 * A315889  Coordination sequence Gal.5.253.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: if n is odd (n = 2k+1) → a(n) = 6 + 8k + 4 *
 *            (k / 5) if n is even (n = 2k) → a(n) = 8k + 4 * ( (k >= 3) ? ((k
 *            - 3) / 5 + 1) : 0 )
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n & 1ULL) {                       // odd
        idx_t k = (n - 1) / 2;
        return (seq_t)6 + (seq_t)8 * k + (seq_t)4 * (k / 5);
    } else {                              // even
        idx_t k = n / 2;
        idx_t count = 0;
        if (k >= 3) {
            count = ((k - 3) / 5) + 1;
        }
        return (seq_t)8 * k + (seq_t)4 * count;
    }
}
