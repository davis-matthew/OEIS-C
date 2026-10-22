/*
 * A313480  Coordination sequence Gal.4.41.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n=1: a(1)=5; for n=2: a(2)=10; for n>=3:
 *            a(n)=a(n-1)+(( (n-3)/4 )%2?5:4)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;          /* a(0) */
    if (n == 0) return val;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t diff;
        if (k == 1) {
            diff = 4;
        } else if (k == 2) {
            diff = 5;
        } else {
            idx_t block = (k - 3) / 4;      /* block number for k>=3 */
            diff = (block & 1) ? 5 : 4;     /* odd block → 5, even→4 */
        }
        val += diff;
    }
    return val;
}
