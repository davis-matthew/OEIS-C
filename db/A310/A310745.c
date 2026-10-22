/*
 * A310745  Coordination sequence Gal.4.7.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1,a(1)=4,a(2)=7,a(3)=11,a(4)=16,a(5)=18; For n>=5, let
 *            m=n-5, q=m/5, r=m%5; a(n)=18+20*q+{0,8,9,13,20}[r]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 7;
    if (n == 3) return 11;
    if (n == 4) return 16;
    if (n == 5) return 18;
    idx_t m = n - 5;
    idx_t q = m / 5;
    idx_t r = m % 5;
    /* incremental sums for the repeating pattern 8,1,4,7,0 */
    const seq_t sum_r[5] = {0, 8, 9, 13, 20};
    return 18 + (seq_t)20 * q + sum_r[r];
}
