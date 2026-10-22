/*
 * A345498  Numbers that are the sum of nine squares in one or more ways.
 * https://oeis.org/A345498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 15 for n > 7. Initial values: a(1)=9, a(2)=12,
 *            a(3)=15, a(4)=17, a(5)=18, a(6)=20, a(7)=21
 * Range:     u8 n <= 240, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 7) {
        static const seq_t init[] = {9,12,15,17,18,20,21};
        return init[n-1];
    }
    return (seq_t)n + 15;
}
