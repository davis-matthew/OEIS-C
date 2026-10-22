/*
 * A311613  Coordination sequence Gal.6.231.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * a(n) = 4*n + 2*c(n) for n ≥ 1 * where c(n) = |{i ∈
 *            [2,n] : i mod 11 ∈ {4,6,8}}| * (i.e. count of numbers 2 ≤ i ≤ n
 *            that are congruent to 4, 6 or 8 modulo 11.) * This is equivalent
 *            to: * a(n) = 4*n + 2* (⌊(n-4)/11⌋ + ⌊(n-6)/11⌋ + ⌊(n-8)/11⌋) for
 *            n ≥ 1, * with the convention ⌊x⌋ = 0 for...
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

    /* base term 4*n */
    seq_t res = (seq_t)4 * (seq_t)n;

    /* Count numbers 2 ≤ i ≤ n with i ≡ 4,6,8 (mod 11) */
    idx_t count = 0;
    const idx_t residues[3] = {4, 6, 8};

    for (int k = 0; k < 3; ++k) {
        idx_t r = residues[k];
        if (n >= r) {
            count += (n - r) / (idx_t)11 + (idx_t)1;
        }
    }

    /* add the extra 2 per such number */
    res += (seq_t)2 * (seq_t)count;

    return res;
}
