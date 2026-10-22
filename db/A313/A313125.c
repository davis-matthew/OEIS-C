/*
 * A313125  Coordination sequence Gal.5.111.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(1)=4; for n>=2,
 *            a(n)=4+44*floor((n-1)/9)+sum_{i=0}^{((n-1)%9)-1}inc[i], where
 *            inc=[5,5,5,6,5,5,5,4,4] (periodic increments)
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

    if (n == 1)
        return (seq_t)4;

    /* increments for n >= 2 */
    static const seq_t inc[9] = {5,5,5,6,5,5,5,4,4};

    idx_t steps = n - 1;                /* number of increments after a(1) */
    idx_t q      = steps / 9;           /* full periods of 9 increments   */
    idx_t r      = steps % 9;           /* remaining increments           */

    seq_t res = (seq_t)4;
    res += (seq_t)q * 44;               /* 44 = sum of one full period */

    for (idx_t i = 0; i < r; ++i)
        res += inc[i];

    return res;
}
