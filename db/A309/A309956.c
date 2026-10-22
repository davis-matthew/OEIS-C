/*
 * A309956  Product of digits of (n written in base 5).
 * https://oeis.org/A309956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = product of the digits of n in base 5
 * Range:     u8 n <= 623, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    idx_t m = n;
    while (m > 0) {
        idx_t d = m % 5;
        prod *= (seq_t)d;
        if (prod == 0)   /* digit 0 encountered, product is zero */
            break;
        m /= 5;
    }
    return prod;
}
