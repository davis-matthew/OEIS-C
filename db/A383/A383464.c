/*
 * A383464  a(n) = 8*n^2 - 5*n + 1.
 * https://oeis.org/A383464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 - 5*n + 1
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 3499, u64 n <= 3499,
 *            u128 n <= 3499, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t res = sn * sn;      // n^2
    res = res * 8;            // 8*n^2
    res = res - sn * 5;       // -5*n
    res = res + 1;            // +1
    return res;
}
