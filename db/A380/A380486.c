/*
 * A380486  Numbers not congruent to {0, 1, 2, 3, 4, 5} mod 30.
 * https://oeis.org/A380486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 6 + 30 * floor((n-1)/24) + ((n-1) mod 24)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n - 1;               /* n >= 1 assumption */
    idx_t block = m / 24;
    idx_t r     = m % 24;
    seq_t value = (seq_t)block * 30 + 6 + (seq_t)r;
    return value;
}
