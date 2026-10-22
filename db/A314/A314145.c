/*
 * A314145  Coordination sequence Gal.4.145.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 5, * a(n) = 5*n + floor((2*n - 3)/5) + 1 for n
 *            >= 2
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 5;

    seq_t nn = (seq_t)n;                       /* promote to seq_t */
    seq_t term = (2 * nn - 3) / 5 + 1;         /* floor division + 1 */
    return 5 * nn + term;
}
