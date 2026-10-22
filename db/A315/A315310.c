/*
 * A315310  Coordination sequence Gal.6.618.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + base[n mod 12] + 2*floor(n/12) – ( (n%12==0 && n>0)
 *            ? 1 : 0 ) where base = {1,1,0,1,0,1,1,1,2,1,2,1}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The periodic offsets for residues 0..11 */
    static const seq_t base[12] = {
        1, 1, 0, 1, 0, 1,
        1, 1, 2, 1, 2, 1
    };

    idx_t  t = n / 12;          /* floor(n/12) */
    idx_t  r = n % 12;          /* n mod 12 */

    /* Compute offset = base[r] + 2*t, subtract 1 when r==0 and n>0 */
    seq_t offset = base[r] + (seq_t)(2) * (seq_t)t;
    if (r == 0 && n > 0)
        offset -= (seq_t)1;

    /* a(n) = 5*n + offset */
    return ((seq_t)5) * (seq_t)n + offset;
}
