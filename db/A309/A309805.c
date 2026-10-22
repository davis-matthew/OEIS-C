/*
 * A309805  Maximum number of nonattacking kings placeable on a hexagonal
 *          board with edge-length n in Glinski's hexagonal chess.
 * https://oeis.org/A309805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - floor(n/2) - floor(n/2)^2
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n must be ≥ 1; for safety we handle n == 0 by returning 0 */
    if (n == 0) return 0;

    seq_t nt = (seq_t)n;       /* convert index to seq_t */
    seq_t f  = nt / 2;         /* floor(n/2) as seq_t */

    /* compute n^2 - floor(n/2) - floor(n/2)^2 */
    return nt * nt - f - f * f;
}
