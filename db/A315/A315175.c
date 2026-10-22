/*
 * A315175  Coordination sequence Gal.4.42.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0; * a(n) = 4*n + 2*(floor(n/7) +
 *            floor((n-1)/7) + 1) for n > 0
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Count indices j ≤ n with j % 7 == 0 (count0) and j % 7 == 1 (count1) */
    idx_t count0 = n / 7;
    idx_t count1 = ((n - 1) / 7) + 1;  /* n ≥ 1, so safe to subtract 1 */

    /* a(n) = 4*n + 2*(count0 + count1) */
    seq_t term4n = (seq_t)4 * (seq_t)n;
    seq_t extra = (seq_t)2 * ((seq_t)count0 + (seq_t)count1);

    return term4n + extra;
}
