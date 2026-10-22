/*
 * A349919  Number of transitive relations on an n-set with exactly two
 *          ordered pairs.
 * https://oeis.org/A349919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - 2*n^3 + 4*n^2 - 3*n)/2 (equivalent to a(n) =
 *            5*C(n,2)+12*C(n,3)+12*C(n,4))
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;           /* cast index to seq_t */
    seq_t n2 = N * N;             /* n^2 */
    seq_t n3 = n2 * N;            /* n^3 */
    seq_t n4 = n3 * N;            /* n^4 */

    seq_t num = n4 - (seq_t)2 * n3 + (seq_t)4 * n2 - (seq_t)3 * N;
    return num / (seq_t)2;
}
