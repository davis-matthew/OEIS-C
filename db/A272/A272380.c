/*
 * A272380  a(n) = n*(120*n^4 - 480*n^3 + 762*n^2 - 556*n + 155).
 * https://oeis.org/A272380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(120*n^4 - 480*n^3 + 762*n^2 - 556*n + 155)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 33, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Order: 0, 1, 2, 3, ...  (a(0) is the first term) */
seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;        /* cast index to seq_t */
    seq_t n2 = n_t * n_t;        /* n^2 */
    seq_t n3 = n2 * n_t;         /* n^3 */
    seq_t n4 = n2 * n2;          /* n^4 */

    seq_t poly = 120 * n4;       /* start with 120*n^4 */
    poly -= 480 * n3;            /* - 480*n^3 */
    poly += 762 * n2;            /* + 762*n^2 */
    poly -= 556 * n_t;           /* - 556*n   */
    poly += 155;                 /* + 155     */

    return n_t * poly;           /* final result n * poly */
}
