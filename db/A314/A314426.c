/*
 * A314426  Coordination sequence Gal.6.527.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1,5,8,13 then for n≥3, a(n) = 13 + 30⋅⌊(n‑3)/7⌋ +
 *            partial[(n‑3) mod 7] where partial = [0,4,8,12,16,23,26,30]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)5;
    if (n == 2u) return (seq_t)8;
    /* n ≥ 3 */
    static const seq_t partial[8] = {0,4,8,12,16,23,26,30};
    idx_t steps  = n - 3u;
    idx_t cycles = steps / 7u;
    idx_t rem    = steps % 7u;
    return (seq_t)13 + (seq_t)cycles * (seq_t)30 + partial[rem];
}
