/*
 * A315465  Coordination sequence Gal.6.157.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + 1 - s[(n-1)%12] * where s =
 *            {0,0,0,0,0,1,2,2,2,2,2,1} for n >= 1. * For n = 0, a(0) = 1
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

    /* Subtraction amounts for (n-1) mod 12 = 0..11. */
    static const unsigned int sub[12] = {
        0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 1
    };

    seq_t base = (seq_t)5 * (seq_t)n + (seq_t)1;
    idx_t r = (n - 1) % 12;
    base -= (seq_t)sub[r];
    return base;
}
