/*
 * A312140  Coordination sequence Gal.4.41.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4. For n>=2: let k = n-2. a(n) = 8 + 36*(k/8) + sum
 *            of the first (k%8) elements of {5,5,5,5,4,4,4,4}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned char inc[8] = {5,5,5,5,4,4,4,4};

    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    idx_t k = n - 2;
    idx_t cycles = k / 8;
    idx_t rem = k % 8;

    seq_t result = (seq_t)8 + (seq_t)36 * (seq_t)cycles;

    for (idx_t i = 0; i < rem; ++i)
        result += inc[i];

    return result;
}
