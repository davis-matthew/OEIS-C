/*
 * A311475  Coordination sequence Gal.6.118.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + count of integers i with 1 ≤ i ≤ n such that i mod
 *            12 ∈ {4,6,7,9}; a(0) = 1. The sequence increases by 4 at each
 *            step except at the positions above, where it increases by 5
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

    seq_t result = 4 * (seq_t)n;          /* 4*n   */
    seq_t cnt = (seq_t)(n / 12) * 4;       /* full 12‑step cycles contribute 4 each */

    idx_t r = n % 12;                     /* remainder of the current cycle */
    if (r >= 4) cnt++;
    if (r >= 6) cnt++;
    if (r >= 7) cnt++;
    if (r >= 9) cnt++;

    return result + cnt;
}
