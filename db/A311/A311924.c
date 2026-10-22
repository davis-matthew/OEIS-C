/*
 * A311924  Coordination sequence Gal.6.118.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n >= 1, a(n) = a(n-1) + s(n), where the step s(n)
 *            depends on n mod 6: if n % 6 == 0 -> s = 4 if n % 6 == 1 -> s =
 *            3 for n==1, else s = 4 if n % 6 == 2 -> s = 4 when ((n-2)/6) is
 *            even, else 5 if n % 6 == 3 -> s = 5 when ((n-3)/6) is even, else
 *            4 if n % 6 == 4 -> s = 4 when ((n-4)/6)...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t value = 1;                /* a(0) */
    if (n == 0) {
        return value;
    }
    for (idx_t i = 1; i <= n; ++i) {
        idx_t r = i % 6;
        seq_t step = 0;
        if (r == 0) {
            step = 4;
        } else if (r == 1) {
            step = (i == 1) ? 3 : 4;
        } else if (r == 2) {
            idx_t k = (i - 2) / 6;
            step = (k & 1) ? 5 : 4;
        } else if (r == 3) {
            idx_t k = (i - 3) / 6;
            step = (k & 1) ? 4 : 5;
        } else if (r == 4) {
            idx_t k = (i - 4) / 6;
            step = (k & 1) ? 5 : 4;
        } else { /* r == 5 */
            idx_t k = (i - 5) / 6;
            step = (k & 1) ? 4 : 5;
        }
        value += step;
    }
    return value;
}
