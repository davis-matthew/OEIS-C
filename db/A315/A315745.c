/*
 * A315745  Coordination sequence Gal.4.137.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)=6*n - (⌊(n-4)/8⌋+1) - (⌊(n-5)/8⌋+1)
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t res = (seq_t)6 * (seq_t)n;

    if (n >= 4)
        res -= (seq_t)((n - 4) / 8 + 1);

    if (n >= 5)
        res -= (seq_t)((n - 5) / 8 + 1);

    return res;
}
