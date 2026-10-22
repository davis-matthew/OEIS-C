/*
 * A312896  Coordination sequence Gal.6.130.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 4*n + Σ_{r∈{2,6,7,11}} ( floor((n - r)/12) + 1 ) - 1
 *            (terms are omitted if n < r). This handles n ≥ 0
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t val = 1 + (seq_t)4 * (seq_t)n;   /* base value 1 + 4*n */

    /* Count the indices of the form 12*k + r (r = 2,6,7,11) that are ≤ n */
    const idx_t residues[4] = {2, 6, 7, 11};
    seq_t cnt = 0;
    for (int i = 0; i < 4; ++i) {
        idx_t r = residues[i];
        if (n >= r) {
            cnt += (seq_t)((n - r) / 12) + 1;
        }
    }

    val += cnt - 1;   /* subtract the -1 adjustment at n = 1 */

    return val;
}
