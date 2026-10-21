/*
 * A243201  Odd octagonal numbers indexed by triangular numbers.
 * https://oeis.org/A243201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^4 + 6*n^3 + 7*n^2 + 4*n + 1
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t term1 = ns * ns + ns + (seq_t)1;          /* n^2 + n + 1 */
    seq_t term2 = 3 * ns * ns + 3 * ns + (seq_t)1;  /* 3*n^2 + 3*n + 1 */
    return term1 * term2;
}
