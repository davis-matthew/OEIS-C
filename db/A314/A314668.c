/*
 * A314668  Coordination sequence Gal.6.115.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 1 + m(n) where m(n) = number of indices i in [1, n]
 *            such that i ≥ 12 and i mod 12 is 0 or 1. Equivalently, m(n) =
 *            (full_cycles * 2) + (rem >= 1) + (rem >= 2) with full_cycles =
 *            floor((n - 11) / 12) and rem = (n - 11) mod 12. For n < 12, m(n)
 *            = 0
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 12U)
        return (seq_t)4 * (seq_t)n + 1;

    /* Number of indices from 12 up to and including n. */
    idx_t offset = n - 11U;          /* = n - 12 + 1 */
    idx_t full   = offset / 12U;
    idx_t rem    = offset % 12U;

    idx_t m = full * 2U
            + (rem >= 1U ? 1U : 0U)
            + (rem >= 2U ? 1U : 0U);

    return (seq_t)4 * (seq_t)n + 1 + (seq_t)m;
}
