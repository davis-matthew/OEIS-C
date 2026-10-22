/*
 * A313538  Coordination sequence Gal.5.110.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(14*n / 3) + c(n) where c(n) = 1 if n mod 9 ∈
 *            {1,2,4} and n ≠ 0, c(n) = 0 otherwise. Special case: a(0) = 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t base = (seq_t)n * 14 / 3;   /* floor(14*n/3) */
    idx_t r = n % 9;
    if (r == 1 || r == 2 || r == 4) return base + 1;
    return base;
}
