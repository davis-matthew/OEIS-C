/*
 * A333692  Scan the binary representation of n from left to right; at each 1,
 *          reverse the bits to the left and including this 1. The resulting
 *          binary representation is that of a(n).
 * https://oeis.org/A333692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0 a(2*n) = 2*a(n) a(2*n+1) = rev_bits( 2*a(n)+1 ) where
 *            rev_bits(x) is the binary-reversal of x (ignoring leading zeros)
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t rev_bits(seq_t x)
{
    seq_t rev = 0;
    while (x)
    {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if ((n & 1) == 0)          /* even n: a(2*k) = 2*a(k) */
    {
        return (seq_t)2 * a(n >> 1);
    }
    else                        /* odd n: a(2*k+1) = rev_bits(2*a(k)+1) */
    {
        seq_t temp = a(n >> 1);
        seq_t val  = (seq_t)2 * temp + 1;
        return rev_bits(val);
    }
}
