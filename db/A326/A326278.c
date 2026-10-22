/*
 * A326278  Number of n-vertex, 2-edge multigraphs that are not nesting.
 *          Number of n-vertex, 2-edge multigraphs that are not crossing.
 * https://oeis.org/A326278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n^3 - 4*n + 3)/6
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;     /* cast index to seq_t */
    seq_t n2 = nn * nn;      /* n^2 */
    seq_t n3 = n2 * nn;      /* n^3 */
    seq_t temp = n3 - ((seq_t)4 * nn) + (seq_t)3;
    seq_t num  = nn * temp;  /* n*(n^3 - 4*n + 3) */
    seq_t res  = num / (seq_t)6;
    return res;
}
