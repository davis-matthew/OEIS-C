/*
 * A313982  Coordination sequence Gal.3.57.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + 2 * (cnt3(n) + cnt5(n)), where * cnt3(n) = # of
 *            integers i (1 ≤ i ≤ n) with i mod 7 = 3, * cnt5(n) = # of
 *            integers i (1 ≤ i ≤ n) with i mod 7 = 5. * a(0) = 1. All
 *            computations use seq_t arithmetic
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Count of numbers ≤ n that are ≡ 3 (mod 7) */
    seq_t cnt3 = 0;
    if (n >= 3U) {
        cnt3 = (seq_t)((n - 3U) / 7U) + 1U;
    }

    /* Count of numbers ≤ n that are ≡ 5 (mod 7) */
    seq_t cnt5 = 0;
    if (n >= 5U) {
        cnt5 = (seq_t)((n - 5U) / 7U) + 1U;
    }

    /* a(n) = 5*n + 2*(cnt3 + cnt5) */
    return (seq_t)5 * (seq_t)n + (seq_t)2 * (cnt3 + cnt5);
}
