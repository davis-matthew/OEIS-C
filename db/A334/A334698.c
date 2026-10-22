/*
 * A334698  a(n) is the total number of points (both boundary and interior
 *          points) in the n-th figure shown in A255011 (meaning the figure
 *          with 4n points on the perimeter), where the interior points are
 *          counted with multiplicity.
 * https://oeis.org/A334698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (17*n^3 - 30*n^2 + 19*n + 4) / 2
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 140, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A334698 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t n2   = n_t * n_t;   /* n^2 */
    seq_t n3   = n2 * n_t;    /* n^3 */

    /* Compute the polynomial 17*n^3 - 30*n^2 + 19*n + 4 */
    seq_t poly = (seq_t)17 * n3
               - (seq_t)30 * n2
               + (seq_t)19 * n_t
               + (seq_t)4;

    return n_t * poly / (seq_t)2;
}
