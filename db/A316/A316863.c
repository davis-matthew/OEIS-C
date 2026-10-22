/*
 * A316863  Number of times 2 appears in the decimal expansion of n.
 * https://oeis.org/A316863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of times digit '2' occurs in decimal expansion of
 *            n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    if (n == 0) return 0;
    idx_t temp = n;
    while (temp > 0) {
        if (temp % 10 == 2) {
            ++count;
        }
        temp /= 10;
    }
    return count;
}
