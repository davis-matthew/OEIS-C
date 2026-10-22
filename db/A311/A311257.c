/*
 * A311257  Coordination sequence Gal.6.115.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)=4*n + (n≥5? ((n-5)/12)+1 : 0) + (n≥8?
 *            ((n-8)/12)+1 : 0)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t res = (seq_t)4 * (seq_t)n;
    if (n >= 5) res += (seq_t)((n - 5) / 12) + (seq_t)1;
    if (n >= 8) res += (seq_t)((n - 8) / 12) + (seq_t)1;
    return res;
}
