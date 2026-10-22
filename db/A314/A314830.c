/*
 * A314830  Coordination sequence Gal.6.150.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(2k)=9k (k≥1); a(2k+1)=9k+5 if k%3==0 or (k%3==1 and
 *            (k/3) is even), otherwise a(2k+1)=9k+4
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    if ((n & 1) == 0) {               /* even index */
        idx_t k = n >> 1;             /* k > 0 here */
        return (seq_t)9 * (seq_t)k;
    } else {                           /* odd index */
        idx_t k = n >> 1;             /* k = floor(n/2) */
        idx_t t = k % 3;
        seq_t d;
        if (t == 0) {
            d = 5;
        } else if (t == 2) {
            d = 4;
        } else { /* t == 1 */
            idx_t half = k / 3;        /* floor(k/3) */
            d = ((half & 1) == 0) ? 5 : 4;
        }
        return (seq_t)9 * (seq_t)k + d;
    }
}
