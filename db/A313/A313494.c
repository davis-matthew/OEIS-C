/*
 * A313494  Coordination sequence Gal.6.155.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = (14*n + f[n mod 12]) / 3 for n >= 1 where f =
 *            {0,1,2,0,-2,-4,0,4,2,0,-2,-1} (indices 0..11) This reproduces
 *            the OEIS sequence A313494
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* f[n mod 12] values (signed integers) */
    static const int f[12] = { 0, 1, 2, 0, -2, -4, 0, 4, 2, 0, -2, -1 };

    unsigned int r = (unsigned int)(n % 12);
    long long temp = (long long)14 * (long long)n + (long long)f[r];
    return (seq_t)(temp / 3);
}
