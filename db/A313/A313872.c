/*
 * A313872  Coordination sequence Gal.6.617.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1, a(1) = 5, a(2) = 10. * For n ≥ 3: * a(n) = 5*n +
 *            \big( ⌊(n-3)/12⌋ + 1 ⌉ + ⌊(n-10)/12⌋ + 1. * The two floor terms
 *            count the indices in [3,n] with residue 3 or 10 modulo 12, *
 *            i.e. the positions where the increment equals 6 instead of 5
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 10;

    seq_t count = 0;
    if (n >= 3) {
        count += ((seq_t)(n - 3) / 12) + 1;
    }
    if (n >= 10) {
        count += ((seq_t)(n - 10) / 12) + 1;
    }
    return (seq_t)5 * (seq_t)n + count;
}
