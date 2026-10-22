/*
 * A326178  Number of subsets of {1..n} whose product is equal to their sum.
 * https://oeis.org/A326178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, dumb
 * Formula:   a(n) = n for n < 3, else n + 1
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)n;
    if (n >= 3) {
        result = result + 1;
    }
    return result;
}
