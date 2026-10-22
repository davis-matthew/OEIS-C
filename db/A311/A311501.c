/*
 * A311501  Coordination sequence Gal.4.38.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(17*n/4) + (n == 0 || n%8 >= 5)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)n * 17) / 4;          /* floor(17*n/4) */
    seq_t add  = (n == 0) ? 1 : ((n % 8) >= 5 ? 1 : 0);
    return base + add;
}
