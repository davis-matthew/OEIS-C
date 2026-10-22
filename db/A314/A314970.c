/*
 * A314970  Coordination sequence Gal.6.341.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = init[n mod 12] + 62 * floor(n/12) * (subtract 1 when n
 *            mod 12 == 0 and n ≥ 12)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First 12 terms of the sequence */
    static const seq_t init[12] = {
        1, 5, 9, 14, 20, 26, 31, 36, 42, 48, 53, 57
    };

    /* Position within the 12‑term cycle and number of completed cycles */
    const idx_t cycle  = n % 12;
    const idx_t cycles = n / 12;

    /* Base value for this residue class */
    seq_t val = init[cycle];

    /* Add the contribution of full cycles */
    val += (seq_t)cycles * (seq_t)62;

    /* For all cycles beyond the first, the residue class 0 is lowered by 1 */
    if (cycle == 0 && cycles > 0)
        val -= (seq_t)1;

    return val;
}
