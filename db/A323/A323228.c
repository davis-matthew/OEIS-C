/*
 * A323228  a(n) = binomial(n + 4, n - 1) + 1.
 * https://oeis.org/A323228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n + 4, n - 1) + 1
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 160, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= 5; ++i) {
        res = res * (seq_t)(n + 5 - i) / (seq_t)i;
    }
    return res + 1;
}
