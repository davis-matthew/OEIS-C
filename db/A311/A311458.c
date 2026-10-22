/*
 * A311458  Coordination sequence Gal.5.95.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 4*n + floor((n+5)/9) + floor((n+3)/9) for n >= 1
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

    seq_t sqn = (seq_t)n;                     /* make sure we use seq_t arithmetic */
    seq_t offset = (sqn + 5) / 9 + (sqn + 3) / 9;  /* floor division for positive numbers */
    return sqn * 4 + offset;
}
