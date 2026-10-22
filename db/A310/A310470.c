/*
 * A310470  Coordination sequence Gal.6.349.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + d[n mod 12] with d = {0,-1,0,0,1,0,0,0,-1,0,0,1}
 *            (and a(0)=1)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const int delta[12] = {0, -1, 0, 0, 1, 0, 0, 0, -1, 0, 0, 1};

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t base = (seq_t)n * (seq_t)5;
    unsigned idx = (unsigned)(n % 12);
    int d = delta[idx];
    if (d > 0) return base + (seq_t)d;
    if (d < 0) return base - (seq_t)(-d);
    return base;
}
