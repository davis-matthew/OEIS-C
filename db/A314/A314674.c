/*
 * A314674  Coordination sequence Gal.4.58.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 1 + floor(n/7) + floor((n-1)/7) with a(0)=1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset case */
    if (n == 0) return 1;

    /* Count the extra +1's that occur at indices 7k and 7k+1 (k>=1) */
    idx_t inc1 = n / 7;          /* indices 7k with 7k <= n */
    idx_t inc2 = (n - 1) / 7;    /* indices 7k+1 with 7k+1 <= n */

    /* Convert everything to the output type before arithmetic */
    seq_t inc  = (seq_t)inc1 + (seq_t)inc2;
    seq_t term = (seq_t)4 * (seq_t)n + 1 + inc;

    return term;
}
