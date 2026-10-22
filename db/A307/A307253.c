/*
 * A307253  Number of triangles larger than size=1 in a matchstick-made
 *          hexagon with side length n.
 * https://oeis.org/A307253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(14*n^2+9*n+2)/4)-6*n^2
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 734, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn     = (seq_t)n;          /* promote index to seq_t */
    seq_t n_sq   = nn * nn;           /* n^2                 */
    seq_t term   = 14 * n_sq + 9 * nn + 2; /* 14*n^2 + 9*n + 2   */
    seq_t numer  = nn * term;         /* n * (14*n^2 + 9*n + 2) */
    seq_t floor_div = numer / 4;      /* floor division by 4 */
    seq_t sub    = 6 * n_sq;          /* 6 * n^2             */
    return floor_div - sub;           /* final result        */
}
