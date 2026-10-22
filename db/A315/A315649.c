/*
 * A315649  Coordination sequence Gal.6.626.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n ≥ 1: a(n) = ⌊ 11·n / 2 ⌋ + δ[n mod 12], where δ
 *            = [0, 1, 1, 0, 0, 0, 0, 1, 0, 1, -1, 0]
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t base = ((seq_t)n * 11) / 2;

    switch (n % 12)
    {
        case 0: return base;
        case 1: return base + (seq_t)1;
        case 2: return base + (seq_t)1;
        case 3: return base;
        case 4: return base;
        case 5: return base;
        case 6: return base;
        case 7: return base + (seq_t)1;
        case 8: return base;
        case 9: return base + (seq_t)1;
        case 10: return base - (seq_t)1;
        case 11: return base;
    }
    /* unreachable, but keep compiler happy */
    return base;
}
