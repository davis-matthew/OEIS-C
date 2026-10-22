/*
 * A312908  Coordination sequence Gal.5.64.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 + 40*floor((n-1)/9) + {0,5,9,13,19,23,27,32,36}[(n-1)
 *            mod 9]
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
    idx_t m = n - 1;
    idx_t q = m / 9;
    idx_t r = m % 9;
    static const seq_t offset[9] = {0, 5, 9, 13, 19, 23, 27, 32, 36};
    return 4 + (seq_t)40 * (seq_t)q + offset[r];
}
