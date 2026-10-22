/*
 * A315342  Coordination sequence Gal.5.303.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 6*n - ( 2 + 4*q + 2*s ) for n >= 2 * where * q = floor(
 *            (n-2) / 8 ) * r = (n-2) % 8 * s = (r >= 5) ? 1 : 0 * a(0) = 1,
 *            a(1) = 6 * This implements the coordination sequence for vertex
 *            type 3 in tiling 5.303.3
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)6;

    idx_t t = n - 2u;
    idx_t q = t / 8u;
    idx_t r = t % 8u;
    idx_t s = (r >= 5u) ? 1u : 0u;

    seq_t res = (seq_t)6 * (seq_t)n;
    seq_t sub = (seq_t)2 + (seq_t)4 * (seq_t)q + (seq_t)2 * (seq_t)s;
    res -= sub;
    return res;
}
