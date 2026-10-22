/*
 * A314253  Coordination sequence Gal.6.627.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 + 6*(n-1) - floor(n/11) - floor((n-1)/11), a(0) = 1
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
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
    seq_t res = (seq_t)5 + (seq_t)6 * (nn - 1);
    res -= nn / (seq_t)11;
    res -= (nn - 1) / (seq_t)11;
    return res;
}
