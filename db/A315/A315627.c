/*
 * A315627  Coordination sequence Gal.6.555.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, 6, 12, 16, 18 for n = 0..4; for n ≥ 5: a(n) = 18 +
 *            32*floor((n-4)/6) + (r==0?0:6*r+2) where r = (n-4) mod 6. The
 *            increment pattern after n = 4 is [8,6,6,6,6,0] with period 6
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 6;
    if (n == 2) return 12;
    if (n == 3) return 16;
    if (n == 4) return 18;                 /* n ≥ 5 handled below */

    /* n ≥ 5 */
    idx_t s = n - 4;
    idx_t cycles = s / 6;                  /* number of full 6‑step cycles   */
    idx_t r      = s % 6;                  /* remainder within the cycle     */

    seq_t result = (seq_t)18
                 + (seq_t)cycles * (seq_t)32;

    if (r != 0)                            /* partial cycle contributes */
        result += (seq_t)(6 * r + 2);

    return result;
}
