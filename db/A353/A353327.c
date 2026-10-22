/*
 * A353327  If n is a multiple of 3, then a(n) = 0, and if n is of the form
 *          3k+r, with r = 1 or 2, then a(n) = 2*k + 3.
 * https://oeis.org/A353327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 0 if n % 3 == 0 else 2 * floor(n / 3) + 3
 * Range:     u8 n <= 381, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n % 3 == 0) {
        return 0;
    } else {
        seq_t k = (seq_t)(n / 3);
        return 2 * k + 3;
    }
}
