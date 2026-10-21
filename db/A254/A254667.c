/*
 * A254667  The nonnegative numbers with 2 instead of 1.
 * https://oeis.org/A254667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n for n >= 3; a(0)=0, a(1)=2, a(2)=2
 * Range:     u8 n <= 255, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)2;
    return (seq_t)n;
}
