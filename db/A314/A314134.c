/*
 * A314134  Coordination sequence Gal.6.624.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * For n >= 1: * a(n) = 5*n + c(n) * where c(n) is the
 *            number of integers k with 2 <= k <= n * such that k mod 12 is in
 *            {2,5,6,7,8,11}. * * c(n) can be computed as * c(n) = (n-1) * -
 *            [floor(n/12) + floor((n-1)/12) * + floor((n-3)/12) + 1 * +
 *            floor((n-4)/12) + 1 * + floor((n-9)/12) + 1 *...
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* number of integers k in [2, n] */
    idx_t total = n - 1;

    /* count of k that give a difference of 5 (residues 0,1,3,4,9,10) */
    idx_t bad = 0;
    bad += n / 12;                     /* residue 0 */
    bad += (n >= 1) ? ((n - 1) / 12) : 0;   /* residue 1 */
    bad += (n >= 3) ? (((n - 3) / 12) + 1) : 0; /* residue 3 */
    bad += (n >= 4) ? (((n - 4) / 12) + 1) : 0; /* residue 4 */
    bad += (n >= 9) ? (((n - 9) / 12) + 1) : 0; /* residue 9 */
    bad += (n >= 10) ? (((n - 10) / 12) + 1) : 0;/* residue 10 */

    idx_t count6 = total - bad;        /* number of steps of +6 */

    return (seq_t)(5 * n + count6);
}
