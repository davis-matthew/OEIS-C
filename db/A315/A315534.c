/*
 * A315534  Coordination sequence Gal.6.624.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(11*n/2) + adjustment * where adjustment = +1 for n
 *            ≡ 1,3,4,7 (mod 12), * adjustment = –1 for n ≡ 8 (mod 12), *
 *            adjustment = 0 otherwise (and a(0)=1)
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t base = ((seq_t)11 * (seq_t)n) / 2;
    idx_t r = n % 12;
    if (r == 1 || r == 3 || r == 4 || r == 7) return base + 1;
    if (r == 8) return base - 1;
    return base;
}
