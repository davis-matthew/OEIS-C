/*
 * A309959  Product of digits of (n written in base 8).
 * https://oeis.org/A309959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Product of digits of n written in base 8
 * Range:     u8 n <= 446, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t prod = 1;
    while (n > 0) {
        idx_t d = n % 8;
        n /= 8;
        prod *= (seq_t)d;
        if (prod == 0) break;
    }
    return prod;
}
