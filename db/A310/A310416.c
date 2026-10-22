/*
 * A310416  Coordination sequence Gal.6.367.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by a(0)=1 and for n>0 a(n) = a(n-1) + d(n) where
 *            d(1)=3, d(2)=6, d(3)=4, d(4)=6, d(5)=4, d(n)= {8,2,6,4}[ (n-6)
 *            mod 4 ] for n≥6
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle base cases directly */
    if (n == 0) return 1;

    static const seq_t cycle[4] = {8, 2, 6, 4}; /* for n≥6 */
    seq_t value = 1;                            /* a(0) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t diff;
        if (i == 1)        diff = 3;
        else if (i == 2)   diff = 6;
        else if (i == 3)   diff = 4;
        else if (i == 4)   diff = 6;
        else if (i == 5)   diff = 4;
        else               diff = cycle[(i - 6) % 4];
        value += diff;
    }

    return value;
}
