/*
 * A315676  Coordination sequence Gal.4.75.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-7)+38 for n >= 8, with a(0)=1, a(1)=6, a(2)=12,
 *            a(3)=17, a(4)=21, a(5)=26, a(6)=32
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[7] = {1, 6, 12, 17, 21, 26, 32};
    if (n < 7)
        return init[n];

    idx_t q = n / 7;        /* number of complete 7-step blocks */
    int    r = n % 7;       /* position within the block */

    seq_t result = init[r] + (seq_t)38 * q;
    if (r == 0)
        result -= (seq_t)1; /* adjustment for the r==0 case (except n==0) */

    return result;
}
