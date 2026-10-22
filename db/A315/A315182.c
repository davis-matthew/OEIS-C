/*
 * A315182  Coordination sequence Gal.6.155.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1≤n≤12 the values are listed; for n≥13 a(n)=56 +
 *            56*floor((n-12)/12) + Σ_{i=0}^{(n-12)%12-1} p[i], where p =
 *            {6,4,4,4,5,5,5,5,4,4,4,6}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pre[13] = {1,6,10,14,18,23,28,33,38,42,46,50,56};
    static const unsigned period[12] = {6,4,4,4,5,5,5,5,4,4,4,6};

    if (n <= 12)
        return pre[n];

    idx_t m = n - 12;
    idx_t cycles = m / 12;
    idx_t rem = m % 12;

    seq_t result = (seq_t)56 + (seq_t)cycles * 56U;
    for (idx_t i = 0; i < rem; ++i)
        result += (seq_t)period[i];
    return result;
}
