/*
 * A313119  Coordination sequence Gal.6.157.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + c(n) where c(n) = -1 if 1 ≤ n mod 12 ≤ 5 0 if n mod
 *            12 = 0 or 6 +1 if 7 ≤ n mod 12 ≤ 11 and a(0) = 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t base = (seq_t)n * (seq_t)5;  /* 5*n */
    idx_t r = n % 12;

    if (r == 0 || r == 6)
        return base;          /* c(n) = 0 */
    if (r <= 5)               /* r = 1..5, c(n) = -1 */
        return base - 1;
    return base + 1;          /* r = 7..11, c(n) = +1 */
}
