/*
 * A399019  Number of empty unit triangles in the hexagonal cross-section of
 *          the first iteration of the generalized Menger sponge with
 *          subdivision 2n+1, cut through the center perpendicular to a space
 *          diagonal.
 * https://oeis.org/A399019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { 3*n*(5*n+3) if n even * 3*n*(3*n+1) if n odd }
 * Range:     u8 n <= 3, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    if ((n & 1U) == 0U) {
        return (seq_t)3 * N * (((seq_t)5 * N) + (seq_t)3);
    } else {
        return (seq_t)3 * N * (((seq_t)3 * N) + (seq_t)1);
    }
}
