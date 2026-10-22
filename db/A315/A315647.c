/*
 * A315647  Coordination sequence Gal.4.139.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n - 2*g(n) * where g(n) is defined by * g(0)=0, g(1)=0,
 *            g(2)=0, g(3)=1, g(4)=1 * for n ≥ 5 let t = n-5, c = t/7, p = t
 *            mod 7 * g(n) = 2 + 2*c + (p ≥ 5 ? 1 : 0) * This yields the
 *            coordination sequence for Gal.4.139.4
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (!n)                     /* a(0) */
        return (seq_t)1;

    idx_t g;  /* group index */
    if (n <= 2)                 /* g = 0 for n = 1,2 */
        g = 0;
    else if (n <= 4)            /* g = 1 for n = 3,4 */
        g = 1;
    else {                      /* n ≥ 5 */
        idx_t t     = n - 5;
        idx_t cycle = t / 7;
        idx_t pos   = t % 7;
        idx_t base  = 2 + 2 * cycle;
        g = (pos <= 4) ? base : base + 1;
    }

    return (seq_t)n * (seq_t)6 - (seq_t)g * (seq_t)2;
}
