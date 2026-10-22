/*
 * A339747  a(n) = (5^(valuation(n, 5) + 1) - 1) / 4.
 * https://oeis.org/A339747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = (5^(valuation(n,5)+1)-1)/4
 * Range:     u8 n <= 624, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    idx_t v = 0;
    while (m % 5 == 0) {
        m /= 5;
        v++;
    }
    seq_t pow = 5;
    for (idx_t i = 0; i < v; ++i) {
        pow *= 5;
    }
    return (pow - 1) / 4;
}
