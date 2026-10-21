/*
 * A279316  Period 7: repeat [0, 1, 2, 3, 3, 2, 1].
 * https://oeis.org/A279316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-7) for n > 6 with base values a(0)=0, a(1)=1, a(2)=2,
 *            a(3)=3, a(4)=3, a(5)=2, a(6)=1
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t pattern[7] = {0, 1, 2, 3, 3, 2, 1};
    return pattern[(int)(n % 7)];
}
