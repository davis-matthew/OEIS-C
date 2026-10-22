/*
 * A315421  Coordination sequence Gal.6.356.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, let k = (n-1)/12 and r = (n-1)%12. Then
 *            a(n) = base[r] + 64*k, where base[0..11] =
 *            {6,11,15,20,26,32,38,44,49,53,58,64}
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    static const seq_t base[12] = {
        6, 11, 15, 20,
        26, 32, 38, 44,
        49, 53, 58, 64
    };

    idx_t k = (n - 1) / 12;
    idx_t r = (n - 1) % 12;

    seq_t k64 = (seq_t)64 * (seq_t)k;
    return base[r] + k64;
}
