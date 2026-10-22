/*
 * A313780  Coordination sequence Gal.3.23.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + ⌊(n+3)/7⌋ for n ≥ 1, a(0) = 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
    {
        return (seq_t)1;
    }

    seq_t nn = (seq_t)n;
    /* floor((n+3)/7) – integer division truncates toward zero */
    seq_t extra = (nn + (seq_t)3) / (seq_t)7;
    return nn * (seq_t)5 + extra;
}
