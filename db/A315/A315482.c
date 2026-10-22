/*
 * A315482  Coordination sequence Gal.6.347.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 62*floor(n/12) + base[n mod 12] (with base[0] = 1), but
 *            for multiples of 12 beyond 0, a(12q) = 62*q
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 0..11 */
    static const seq_t base[12] =
    { 1, 6, 11, 16, 22, 26, 31, 36, 40, 46, 51, 56 };

    if (n == 0)
        return base[0];

    idx_t q = n / 12;           /* floor division by 12 */
    idx_t r = n % 12;           /* remainder */

    /* Multiply q by 62 in seq_t type */
    seq_t add = (seq_t)q * 62;

    if (r == 0)
        return add;            /* multiples of 12 (n>0) */

    return base[r] + add;       /* otherwise add the base value */
}
