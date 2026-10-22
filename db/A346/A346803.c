/*
 * A346803  Numbers that are the sum of nine squares in ten or more ways.
 * https://oeis.org/A346803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 69 for n > 7; a(1)=63, a(2)=65, a(3)=68, a(4)=71,
 *            a(5)=72, a(6)=74, a(7)=75
 * Range:     u8 n <= 186, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t first[7] = {63, 65, 68, 71, 72, 74, 75};
    if (n <= 7) return first[n - 1];
    return (seq_t)n + (seq_t)69;
}
