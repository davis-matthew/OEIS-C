/*
 * A305859  Numbers that are congruent to {1, 3, 11} mod 12.
 * https://oeis.org/A305859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2*n + 6*floor(n/3) - 1
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n is 1-based index as in OEIS */
    /* The term can be computed directly from the closed form. */
    seq_t term = (seq_t)2 * n + (seq_t)6 * (n / 3) - 1;
    return term;
}
