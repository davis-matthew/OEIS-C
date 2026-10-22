/*
 * A345508  Numbers that are the sum of ten squares in one or more ways.
 * https://oeis.org/A345508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10,13,16,18,19,21,22,24,25 for n≤9; a(n) = n + 16 for n≥9
 * Range:     u8 n <= 239, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t init[9] = {10, 13, 16, 18, 19, 21, 22, 24, 25};
    if (n <= 9) {
        return init[n - 1];
    }
    return (seq_t)(n + 16);
}
