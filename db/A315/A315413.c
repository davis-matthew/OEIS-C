/*
 * A315413  Coordination sequence Gal.6.157.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + f(n) where f(n) = +1 if n == 0 or n mod 12 == 1 or
 *            2 -1 if n mod 12 == 10 or 11 0 otherwise
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for the first term */
    if (n == 0u)
        return (seq_t)1;

    seq_t base = (seq_t)5 * (seq_t)n;
    idx_t r = n % (idx_t)12u;

    if (r == 1u || r == 2u)
        return base + (seq_t)1;
    if (r == 10u || r == 11u)
        return base - (seq_t)1;

    return base;
}
