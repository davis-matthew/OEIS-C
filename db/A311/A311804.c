/*
 * A311804  Coordination sequence Gal.6.216.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordinate sequence for Gal.6.216.4 (OEIS A311804)
   a(0) = 1
   a(n) = 4n + floor((n-3)/11)+1 + floor((n-9)/11)+1   for n ≥ 1,
   (with the floor‑terms zero when the numerators are negative)   */
seq_t a(idx_t n)
{
    if (n == 0)                    /* a(0) = 1 */
        return (seq_t)1;

    /* start with the linear part 4n */
    idx_t val = 4 * n;

    /* extra +1 for each integer ≡3 (mod 11) in 2..n */
    if (n >= 3)
        val += (n - 3) / 11 + 1;

    /* extra +1 for each integer ≡9 (mod 11) in 2..n */
    if (n >= 9)
        val += (n - 9) / 11 + 1;

    return (seq_t)val;
}
