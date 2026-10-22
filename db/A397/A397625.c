/*
 * A397625  Coordination sequence of the bcs net.
 * https://oeis.org/A397625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n*n + 2 for n >= 3; a(0)=1, a(1)=6, a(2)=24
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)24;
    seq_t sn = (seq_t)n;
    seq_t res = sn * sn;      /* n^2 */
    res = res * (seq_t)6;     /* 6*n^2 */
    res = res + (seq_t)2;     /* +2 */
    return res;
}
