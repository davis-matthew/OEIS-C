/*
 * A313062  Coordination sequence Gal.6.154.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(n)=a(n-1)+p[(n-2) mod 11] with
 *            p={5,5,4,5,6,5,4,5,5,4,4}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pat[11] = {5,5,4,5,6,5,4,5,5,4,4};
    if (n == 0) return 1;
    if (n == 1) return 4;

    seq_t res = 4;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        res += pat[(i - 2) % 11];
    }
    return res;
}
