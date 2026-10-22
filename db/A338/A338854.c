/*
 * A338854  Product of the nonzero digits of (n written in base 4).
 * https://oeis.org/A338854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of the nonzero digits of (n written in base 4)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t temp = n;
    while (temp > 0)
    {
        unsigned d = temp % 4;
        if (d)
            result *= (seq_t)d;
        temp /= 4;
    }
    return result;
}
