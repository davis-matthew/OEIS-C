/*
 * A340051  Mixed-radix representation of n where the least significant digit
 *          is in base 3 and other digits are in base 2.
 * https://oeis.org/A340051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 10*A007088(floor(n/3)) + (n mod 3)
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 1535, u64 n <= 3072,
 *            u128 n <= 3072, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 3;
    seq_t val = 0;
    seq_t mult = 1;
    while (m)
    {
        if (m & 1)
            val += mult;
        mult *= 10;
        m >>= 1;
    }
    return val * 10 + (seq_t)(n % 3);
}
