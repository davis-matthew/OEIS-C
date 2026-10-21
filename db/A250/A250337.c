/*
 * A250337  Number of length 1+5 0..n arrays with every six consecutive terms
 *          having the maximum of some three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n^5 + 10*n^4 + 15*n^3 + 11*n^2 + 4*n + 1 (valid for n ≥
 *            1; a(0) is unused and returns 0)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 67, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* unused with offset 1 */

    /* cast the index once to the sequence type */
    seq_t N = (seq_t)n;

    /* Horner's rule: (((((3)*n + 10)*n + 15)*n + 11)*n + 4)*n + 1 */
    seq_t r = (seq_t)3;       /* coefficient of n^5 */
    r = r * N + (seq_t)10;     /* coefficient of n^4 */
    r = r * N + (seq_t)15;     /* coefficient of n^3 */
    r = r * N + (seq_t)11;     /* coefficient of n^2 */
    r = r * N + (seq_t)4;      /* coefficient of n   */
    r = r * N + (seq_t)1;      /* constant term     */

    return r;
}
