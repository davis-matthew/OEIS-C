/*
 * A313975  Coordination sequence Gal.4.145.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1: a(n) = 5*n + 2*t, where t counts indices k
 *            in [2,n] with k mod 5 = 3. Equivalently, t = 0 for n < 3;
 *            otherwise t = (n-3)/5 + 1
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t t;
    if (n < 3)
        t = 0;
    else
        t = (n - 3) / 5 + 1;

    return (seq_t)5 * (seq_t)n + (seq_t)2 * (seq_t)t;
}
