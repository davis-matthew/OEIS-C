/*
 * A313692  Coordination sequence Gal.5.136.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1, let v(n) = 5n – a(n). The values of v(n)
 *            form blocks: 0 for n = 1..3, 1 for n = 4..6, then alternately 2
 *            for seven terms, 3 for three terms, 4 for seven terms, 5 for
 *            three terms, … For n ≥ 7 we can compute it as k = n – 7; c = k /
 *            10; // complete 10‑term cycles r = k % 10; //...
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

    seq_t sn = (seq_t)n;
    seq_t v;

    if (n < 7)  /* manual handling for n = 1..6 */
    {
        if (n <= 3)
            v = 0;          /* n = 1,2,3 */
        else
            v = 1;          /* n = 4,5,6 */
    }
    else        /* n ≥ 7 */
    {
        idx_t k = n - 7;
        idx_t cycle = k / 10;
        idx_t rem   = k % 10;
        idx_t base  = 2 + 2 * cycle;
        v = (rem <= 6) ? base : base + 1;
    }

    return (seq_t)5 * sn - v;
}
