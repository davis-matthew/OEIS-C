/*
 * A313729  Coordination sequence Gal.6.134.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, a(n) = 5*n – C, where C is the number of
 *            integers k in [2,n] with k ≡ 6 or 7 (mod 12)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    seq_t c = 0;
    if (n >= 6)
        c += (seq_t)((n - 6) / 12) + 1;
    if (n >= 7)
        c += (seq_t)((n - 7) / 12) + 1;
    return 5 * nn - c;
}
