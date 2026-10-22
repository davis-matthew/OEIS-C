/*
 * A338863  Product of the nonzero digits of (n written in base 6).
 * https://oeis.org/A338863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of the nonzero base-6 digits of n, with a(0) = 1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    while (n > 0) {
        idx_t d = n % 6;
        if (d != 0)
            prod *= (seq_t)d;
        n /= 6;
    }
    return prod;
}
