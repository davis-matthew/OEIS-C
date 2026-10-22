/*
 * A313510  Coordination sequence Gal.3.13.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n >= 1, let q = floor(n / 6) and r = n % 6; let m
 *            = 2*q + (r >= 3) + (r >= 4); then a(n) = 5*n - m
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t q = n / 6;
    idx_t r = n % 6;
    idx_t m = q * 2;
    if (r >= 3) m++;
    if (r >= 4) m++;

    return (seq_t)5 * (seq_t)n - (seq_t)m;
}
