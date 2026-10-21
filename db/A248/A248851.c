/*
 * A248851  a(n) = ( 2*n*(2*n^2 + 9*n + 14) + (-1)^n - 1 )/16.
 * https://oeis.org/A248851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( 2*n*(2*n^2 + 9*n + 14) + (-1)^n - 1 )/16
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t inner = (seq_t)2 * sn * sn + (seq_t)9 * sn + 14;
    seq_t t = (seq_t)2 * sn * inner;
    if (n & 1) {
        t -= 2;
    }
    return t / 16;
}
