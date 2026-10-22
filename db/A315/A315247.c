/*
 * A315247  Coordination sequence Gal.5.87.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + (n%5==1 ? 1 : n%5==4 ? -1 : 0), a(0)=1
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

    seq_t tn = (seq_t)n;          /* convert index to seq_t to avoid overflow */
    seq_t ans = tn * (seq_t)5;    /* base linear term */

    /* adjust according to n mod 5 */
    switch ((unsigned)(n % 5))
    {
        case 1: ans += 1; break;
        case 4: ans -= 1; break;
        default: break;
    }

    return ans;
}
