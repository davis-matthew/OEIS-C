/*
 * A315361  Coordination sequence Gal.5.328.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n >= 1: if n mod 3 = 0: a(n) = 6*n if n mod 3 = 1:
 *            a(n) = 6*n - 2*floor((n-1)/3) if n mod 3 = 2: a(n) = 6*n -
 *            2*floor((n+1)/3)
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
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
    seq_t val;

    /* use integer division by 3, which is floor for unsigned values */
    if ((n % 3) == 0) {
        val = sn * (seq_t)6;
    } else if ((n % 3) == 1) {
        val = sn * (seq_t)6;
        seq_t sub = (seq_t)((n - 1) / 3) * (seq_t)2;
        val -= sub;
    } else { /* n % 3 == 2 */
        val = sn * (seq_t)6;
        seq_t sub = (seq_t)((n + 1) / 3) * (seq_t)2;
        val -= sub;
    }
    return val;
}
