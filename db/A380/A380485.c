/*
 * A380485  Numbers congruent to {0, 1, 2, 3, 4, 5} mod 30.
 * https://oeis.org/A380485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*floor((n-1)/6) + ((n-1) mod 6)
 * Range:     u8 n <= 54, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* no defined value for n < 1 */
    idx_t k = n - 1;
    seq_t q = ((seq_t)30 * ((seq_t)k / 6));
    seq_t r = ((seq_t)k % 6);
    return q + r;
}
