/*
 * A310491  Coordination sequence Gal.6.154.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4; for n>=2, a(n)=a(n-1)+p[(n-2) mod 11], where
 *            p={6,6,4,4,4,4,4,6,6,4,4}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    static const seq_t p[11] = {6, 6, 4, 4, 4, 4, 4, 6, 6, 4, 4};

    seq_t val = (seq_t)4;
    for (idx_t k = 2; k <= n; ++k)
        val += p[(k - 2) % 11];
    return val;
}
