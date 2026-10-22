/*
 * A309788  Product of digits of (n written in base 9).
 * https://oeis.org/A309788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = product of the base-9 digits of n; a(0) = 0
 * Range:     u8 n <= 403, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t prod = 1;
    while (n > 0) {
        idx_t digit = n % 9;
        prod *= (seq_t)digit;
        if (prod == 0)
            break;                   /* early exit if zero digit found */
        n /= 9;
    }
    return prod;
}
