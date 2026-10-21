/*
 * A279019  Least possible number of diagonals of simple convex polyhedron
 *          with n faces.
 * https://oeis.org/A279019
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-4)*(n-5) (for n >= 4)
 * Range:     u8 n <= 20, u16 n <= 260, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* undefined below offset; return 0 safely */
    seq_t nn = (seq_t)n;          /* cast index to seq_t to avoid unsigned wrap‑around */
    return (nn - 4) * (nn - 5);   /* compute directly, allowing overflow on small seq_t */
}
