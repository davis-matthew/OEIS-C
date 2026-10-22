/*
 * A313115  Coordination sequence Gal.4.43.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n - f(n), where f(n) = 2*(n/8) if n % 8 == 0 1 +
 *            2*(n/8) otherwise. a(0) = 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t q = n / 8;
    seq_t f = (seq_t)(n % 8 == 0 ? 2 * q : 1 + 2 * q);
    seq_t nn = (seq_t)n;
    return 5 * nn - f;
}
