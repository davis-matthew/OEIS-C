/*
 * A309953  Product of digits of (n written in base 3).
 * https://oeis.org/A309953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = product of the digits of n when written in base 3
 * Range:     u8 n <= 6559, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t prod = (seq_t)1;
    idx_t m = n;
    while (m > 0) {
        idx_t digit = m % 3;
        if (digit == 0)
            return (seq_t)0;
        prod *= (seq_t)digit;
        m /= 3;
    }
    return prod;
}
