/*
 * A305989  Numbers in binary reversed.
 * https://oeis.org/A305989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A004086(A007088(n)) (reverse binary digits of n,
 *            interpret as decimal)
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 1024, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t rev = 0;
    idx_t x = n;
    while (x)
    {
        rev = rev * 10 + (seq_t)(x & 1);
        x >>= 1;
    }
    return rev;
}
