/*
 * A311801  Coordination sequence Gal.6.115.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n >= 1: a(n) = 4*n + [n >= 3] * (⌊(n-3)/12⌋ + 1) +
 *            [n >= 10] * (⌊(n-10)/12⌋ + 1) (The brackets denote that the term
 *            is added only if the preceding condition is true.) This is
 *            equivalent to: a(0) = 1 a(1) = 4 a(n) = a(n-1) + 4 for n >= 2
 *            a(n) = a(n-1) + 5 for n ≡ 3 or 10 (mod 12), n >= 2
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* base value 4*n */
    seq_t val = (seq_t)4 * (seq_t)n;

    /* add counts of indices with residue 3 mod 12 */
    if (n >= 3) {
        val += ((seq_t)(n - 3) / 12) + 1;
    }

    /* add counts of indices with residue 10 mod 12 */
    if (n >= 10) {
        val += ((seq_t)(n - 10) / 12) + 1;
    }

    return val;
}
