/*
 * A313280  Coordination sequence Gal.3.15.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=9, a(3)=15, a(4)=21, a(5)=26; for n≥6:
 *            a(n)=a(n-1)+{4,4,5,6,6,5}[(n-6)%6]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 5) {
        seq_t val = 1;
        seq_t inc = 3;
        for (idx_t i = 1; i <= n; ++i) {
            val += inc;
            switch (i) {          /* update increment for next step */
                case 1: inc = 5; break;
                case 2: inc = 6; break;
                case 3: inc = 6; break;
                case 4: inc = 5; break;
                default: inc = 0; break;   /* only used for i==5 */
            }
        }
        return val;
    }
    /* n >= 6 */
    static const seq_t prefix[6] = {0, 4, 8, 13, 19, 25}; /* cumulative sums of cycle */
    seq_t base   = 26;                     /* a(5) */
    idx_t m      = n - 5;                   /* terms beyond n=5 */
    seq_t cycles = (seq_t)(m / 6);          /* full cycles of length 6 */
    idx_t rem    = m % 6;                   /* remaining terms in last cycle */

    base += (seq_t)30 * cycles + prefix[rem];
    return base;
}
