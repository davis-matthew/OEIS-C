/*
 * A281553  Write n in binary reflected Gray code, rotate one binary place to
 *          the right and convert the code back to decimal.
 * https://oeis.org/A281553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n ^ (n >> 1)) rotated right by 1 bit
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Gray code of n */
    seq_t g = ((seq_t)n) ^ (((seq_t)n) >> 1);
    if (g == 0)
        return 0;

    /* length of binary representation of n (or g) */
    idx_t len = (n == 0) ? 1 : (64 - __builtin_clzll(n));

    /* rotate g one bit to the right */
    seq_t low = g & 1;
    seq_t rot = (g >> 1) | (low << (len - 1));
    return rot;
}
