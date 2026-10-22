/*
 * A315315  Coordination sequence Gal.6.198.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + d[n mod 12] where d = {1,0,1,1,0,0,0,-1,-1,0,-1,0}
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

    static const int delta[12] = {1, 0, 1, 1, 0, 0, 0, -1, -1, 0, -1, 0};

    idx_t mod = n % 12;
    int d = (mod == 0) ? delta[11] : delta[mod - 1];

    seq_t res = (seq_t)n * 5;
    if (d >= 0)
        res += (seq_t)d;
    else
        res -= (seq_t)(-d);

    return res;
}
