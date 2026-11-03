/*
 * A379007  a(n) = (n^2) XOR ((n^2)-1).
 * https://oeis.org/A379007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = (n*n) ^ ((n*n)-1)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 112 terms through n = 9494 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* safety guard for n<1 */
    seq_t nn = (seq_t)n * (seq_t)n;
    return nn ^ (nn - 1);
}
