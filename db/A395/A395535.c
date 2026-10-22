/*
 * A395535  a(n) = (n XOR n*2+1) for odd n, (n XOR n/2) for even n.
 * https://oeis.org/A395535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = (n XOR (n*2+1)) for odd n, (n XOR n/2) for even n
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0)   /* even n: a(n) = n XOR (n/2) */
        return nn ^ (nn >> 1);
    else                /* odd n : a(n) = n XOR (2n+1) */
        return nn ^ ((nn << 1) + 1);
}
