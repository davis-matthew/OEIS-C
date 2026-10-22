/*
 * A309958  Product of digits of (n written in base 7).
 * https://oeis.org/A309958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of the digits of n when written in base 7
 * Range:     u8 n <= 929, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    seq_t prod = 1;
    idx_t tmp = n;
    while (tmp > 0) {
        seq_t digit = (seq_t)(tmp % 7);
        prod *= digit;
        tmp /= 7;
    }
    return prod;
}
