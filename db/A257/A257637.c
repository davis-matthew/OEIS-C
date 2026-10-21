/*
 * A257637  Maximal number of edges in an n-vertex triangle-free graph with
 *          maximal degree at most 4.
 * https://oeis.org/A257637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * a(n) = floor(n^2/4) for 1 <= n <= 8 * a(9) = 17 * a(n) = 2*n
 *            for n >= 10
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= (idx_t)8) {
        seq_t temp = (seq_t)n * (seq_t)n;
        return temp / (seq_t)4;
    } else if (n == (idx_t)9) {
        return (seq_t)17;
    } else { /* n >= 10 */
        return (seq_t)2 * (seq_t)n;
    }
}
