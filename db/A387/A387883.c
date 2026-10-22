/*
 * A387883  a(n) is the number of regions into which the plane is divided by
 *          n^2 circles of radius 1, the centers of which are located at the
 *          nodes of a square lattice n X n.
 * https://oeis.org/A387883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n^2 - 8*n + 2 for n > 1; a(0) = 1, a(1) = 2
 * Range:     u8 n <= 6, u16 n <= 97, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) {
        return (n == 0) ? (seq_t)1 : (seq_t)2;
    }
    seq_t nn = (seq_t)n;
    return 7*nn*nn - 8*nn + 2;
}
