/*
 * A348363  The 1's in the binary expansion of a(n) encode the distances
 *          between the 1's in the binary expansion of n.
 * https://oeis.org/A348363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = bitwise OR of 2^d for all d ≥ 0 such that (n & (n >> d))
 *            != 0
 * Range:     u8 n <= 256, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t res = 0;
    const unsigned maxbits = sizeof(seq_t) * 8u;

    for (unsigned d = 0; d < 64; ++d)
    {
        if ((n >> d) & n)
        {
            if (d < maxbits)
                res |= ((seq_t)1 << d);
        }
    }

    return res;
}
