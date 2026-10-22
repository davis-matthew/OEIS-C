/*
 * A389313  a(n) is the smallest m such that for every red-blue edge-coloring
 *          of the graph K_{m} there exists either a red or a blue n-cycle;
 *          Ramsey number r(C_n, C_n).
 * https://oeis.org/A389313
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 for n = 3 or 4; a(n) = 2*n - 1 if n is odd; a(n) =
 *            3*n/2 - 1 if n is even (for n >= 5)
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return 0;          /* values below the offset are undefined */
    if (n == 3 || n == 4) return 6;
    if (n & 1) return (seq_t)n * 2 - 1;
    return (seq_t)n * 3 / 2 - 1;
}
