/*
 * A288249  2 * largest possible area of a simple n-sided lattice polygon
 *          whose vertex coordinates x and y are both independent permutations
 *          of the integers 1 ... n, subject to the condition that none of its
 *          edges are mutually parallel.
 * https://oeis.org/A288249
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*n^2 – 9*(n-1) for n≥23; a(3..22) from pre‑computed
 *            table
 * Range:     u8 n <= 13, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t init[20] = {
        3, 8, 18, 28, 44, 65, 88, 118, 151, 189, 226, 272, 323, 377,
        433, 493, 558, 627, 701, 778
    };                       /* indices: n=3→0, n=22→19 */
    if (n < 3)
        return 0;                   /* Undefined for n<3, return 0 by convention. */
    if (n <= 22)
        return init[n - 3];
    seq_t nn = (seq_t)n;
    return 2 * nn * nn - 9 * (nn - 1);
}
