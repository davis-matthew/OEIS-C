/*
 * A304165  a(n) = 324*n^2 - 336*n + 102 (n >= 1).
 * https://oeis.org/A304165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 324*n^2 - 336*n + 102 (n >= 1)
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* Not part of the defined sequence */
    seq_t nn = (seq_t)n;
    return (seq_t)(324 * nn * nn - 336 * nn + 102);
}
