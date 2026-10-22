/*
 * A334697  a(n) is the number of interior points in the n-th figure shown in
 *          A255011 (meaning the figure with 4n points on the perimeter),
 *          counted with multiplicity.
 * https://oeis.org/A334697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(17*n^3-30*n^2+19*n-4)/2
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 150, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;        /* n^2 */
    seq_t n3 = n2 * nn;        /* n^3 */
    /* expr = 17*n^3 - 30*n^2 + 19*n - 4  (always even) */
    seq_t expr = (seq_t)17 * n3 - (seq_t)30 * n2 + (seq_t)19 * nn - (seq_t)4;
    seq_t half_expr = expr / 2;/* divide by 2 before multiplying by n to avoid overflow */
    return nn * half_expr;
}
