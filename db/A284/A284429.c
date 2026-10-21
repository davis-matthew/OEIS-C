/*
 * A284429  A quasilinear solution to Hofstadter's Q recurrence.
 * https://oeis.org/A284429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = 3; a(3k+1) = 3k+2; a(3k+2) = 1; offset 1
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n >= 1 is assumed by the OEIS offset */
    idx_t mod = n % 3;
    if (mod == 0) {
        return (seq_t)3;
    } else if (mod == 2) {
        return (seq_t)1;
    } else { /* mod == 1 */
        seq_t k = (seq_t)(n / 3);
        return k * 3 + 2;
    }
}
