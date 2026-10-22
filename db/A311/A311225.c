/*
 * A311225  Coordination sequence Gal.6.216.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 4*n for 1 ≤ n ≤ 5 a(n) = 4*n + 2*((n-6)/11 + 1)
 *            for n ≥ 6
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
    if (n <= 5)
        return 4 * (seq_t)n;
    idx_t block = (n - 6) / 11;          /* block index for n ≥ 6 */
    seq_t offset = 2 * ((seq_t)(block + 1));
    return 4 * (seq_t)n + offset;
}
