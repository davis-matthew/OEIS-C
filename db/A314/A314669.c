/*
 * A314669  Coordination sequence Gal.6.216.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 1 + floor((n-1)/11) + floor(n/11), for n >= 1; a(0)
 *            = 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    seq_t sn = (seq_t)n;
    seq_t val = 4 * sn + 1;
    val += (seq_t)((n - 1) / 11);
    val += (seq_t)(n / 11);
    return val;
}
