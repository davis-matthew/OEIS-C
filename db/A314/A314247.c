/*
 * A314247  Coordination sequence Gal.3.50.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1: a(n) = 5 + ⌊(n‑1)/6⌋·34 + p[(n‑1) mod 6],
 *            where p = {0, 6, 12, 18, 24, 29}. This reproduces the observed
 *            differences 4, (6,6,6,6,5,5) repeating
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    uint64_t m    = n - 1;          /* number of increments after a(1) */
    uint64_t full = m / 6;          /* full 6‑term cycles */
    uint64_t rem  = m % 6;          /* remaining terms */

    static const seq_t prefix[6] = { 0, 6, 12, 18, 24, 29 };
    return (seq_t)5 + ((seq_t)full) * 34 + prefix[rem];
}
