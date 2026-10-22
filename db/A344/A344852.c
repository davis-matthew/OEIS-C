/*
 * A344852  Number of rooted binary trees with n leaves with minimal Symmetry
 *          Nodes Index (SNI) or, equivalently, with the maximal number of
 *          symmetry nodes.
 * https://oeis.org/A344852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*A000120(n) - 3)!!
 * Range:     u8 n <= 62, u16 n <= 254, u32 n <= 4094, u64 n <= 9468,
 *            u128 n <= 9468, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t popcnt(idx_t x) {
#ifdef __GNUC__
    return __builtin_popcountll(x);
#else
    idx_t c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
#endif
}

seq_t a(idx_t n) {
    idx_t w = popcnt(n);
    if (w <= 1) return (seq_t)1;
    seq_t r = 1;
    idx_t m = 2 * w - 3;          /* largest odd argument of double factorial */
    for (idx_t i = 1; i <= m; i += 2) {
        r *= (seq_t)i;
    }
    return r;
}
