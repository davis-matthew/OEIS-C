/*
 * A338882  Product of the nonzero digits of (n written in base 9).
 * https://oeis.org/A338882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of nonzero base-9 digits of n, a(0)=1
 * Range:     u8 n <= 403, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t temp = n;
    while (temp > 0) {
        idx_t digit = temp % 9;
        if (digit != 0)
            prod *= (seq_t)digit;
        temp /= 9;
    }
    return prod;
}
