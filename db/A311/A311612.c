/*
 * A311612  Coordination sequence Gal.5.98.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0; otherwise a(n) = 4*n + 2*C(n) where C(n) =
 *            number of integers k in {1,…,n} with k mod 9 == 4 or 6
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t q = n / 9;      /* full blocks of 9 */
    idx_t r = n % 9;      /* remaining part */

    /* Count how many of {4,6} are within the remainder block */
    idx_t count = 2 * q;  /* two residues per full block */
    if (r >= 4) count++;  /* residue 4 present */
    if (r >= 6) count++;  /* residue 6 present */

    return 4 * (seq_t)n + 2 * (seq_t)count;
}
