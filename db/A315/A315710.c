/*
 * A315710  Coordination sequence Gal.6.628.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n ≥ 1, define c(n) = 1 + ⌊(n-3)/12⌋ if n ≥ 3, 0
 *            otherwise; d(n) = 1 + ⌊(n-10)/12⌋ if n ≥ 10, 0 otherwise; Then
 *            a(n) = 6*n – c(n) – d(n)
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0UL)
        return (seq_t)1UL;

    /* Convert index to seq_t for arithmetic. */
    seq_t nn = (seq_t)n;

    seq_t cnt = 0UL;

    if (n >= 3UL) {
        /* (n - 3) is safe because n ≥ 3. */
        cnt += ((seq_t)(n - 3UL) / 12UL) + 1UL;
    }
    if (n >= 10UL) {
        cnt += ((seq_t)(n - 10UL) / 12UL) + 1UL;
    }

    return ((seq_t)6UL * nn) - cnt;
}
