/*
 * A311426  Coordination sequence Gal.6.115.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 2*(n/12) + (n%12>3) + (n%12>8) for n > 0, a(0) = 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t base = n / 12;
    idx_t rem  = n % 12;

    idx_t extra = 0;
    if (rem > 3) ++extra;
    if (rem > 8) ++extra;

    seq_t sn = (seq_t)n;
    seq_t term = (seq_t)base * 2 + (seq_t)extra;

    return 4 * sn + term;
}
