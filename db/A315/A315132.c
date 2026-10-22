/*
 * A315132  Coordination sequence Gal.6.547.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n > 0: let k = n / 6 (integer division) r = n % 6
 *            a(n) = 28*k + offset(r), where offset(1) = 6 offset(2) = 12
 *            (except when n = 2, a(2) = 10) offset(3) = 12 offset(4) = 20
 *            offset(5) = 20 offset(0) = 0 (a(6k) = 28*k, except a(0) = 1)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    idx_t k = n / 6;
    idx_t r = n % 6;
    seq_t base = (seq_t)k * 28;

    switch (r) {
        case 0: return base;
        case 1: return base + 6;
        case 2: return (n == 2) ? (seq_t)10 : base + 12;
        case 3: return base + 12;
        case 4: return base + 20;
        case 5: return base + 20;
    }
    /* unreachable but satisfies compiler */
    return base;
}
