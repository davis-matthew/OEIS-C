/*
 * A315756  Coordination sequence Gal.6.637.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, if n = 0. For n ≥ 1, a(n) = 6*n – 2 * (c1 + c2), where
 *            c1 = floor((n-5)/11) + 1, for n ≥ 5, else 0 c2 = floor((n-7)/11)
 *            + 1, for n ≥ 7, else 0
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the coordination sequence for tiling Gal.6.637.6   */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                      /* a(0) = 1 */

    /* 6*n, computed in the appropriate integer type */
    seq_t six = (seq_t)n * 6u;

    /* Counts of breakpoints at positions 5+11m and 7+11m */
    idx_t c1 = 0;
    idx_t c2 = 0;
    if (n >= 5)
        c1 = (n - 5) / 11 + 1;
    if (n >= 7)
        c2 = (n - 7) / 11 + 1;

    /* Subtract 2 times the total number of breakpoints */
    seq_t sub = ((seq_t)c1 + (seq_t)c2) * 2u;

    return six - sub;
}
