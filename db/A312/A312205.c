/*
 * A312205  Coordination sequence Gal.4.43.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=8, a(3)=13, a(4)=19, a(5)=25, a(6)=30; for
 *            n≥7, a(n)=30+38*floor((n-6)/8)+[0,4,8,12,16,21,27,33,38][(n-6)
 *            mod 8]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 6) {
        static const seq_t init[6] = {1, 4, 8, 13, 19, 25};
        return init[n];
    }
    if (n == 6) return (seq_t)30;

    idx_t m    = n - 6;
    idx_t cycles = m / 8;
    idx_t rem    = m % 8;

    seq_t result = (seq_t)30 + (seq_t)cycles * (seq_t)38;
    static const seq_t prefix[9] = {0, 4, 8, 12, 16, 21, 27, 33, 38};
    result += prefix[rem];
    return result;
}
