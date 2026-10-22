/*
 * A316047  Coordination sequence Gal.6.374.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A316047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n)=a(n-1)+x where x = 5 if n-1==0, x = 3 if
 *            (n-1)%9==1 or 7, otherwise x = 6
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;                /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t k = i - 1;                 /* previous index */
        seq_t inc = 6;                   /* default increment */
        if (k == 0)
            inc = 5;
        else if ((k % 9) == 1 || (k % 9) == 7)
            inc = 3;
        res += inc;
    }
    return res;
}
