/*
 * A292045  Wiener index of the n X n X n grid graph.
 * https://oeis.org/A292045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1)*(n + 1)*n^5/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 26, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sn = (seq_t)n;

    /* (n^2 - 1) */
    seq_t a_term = (sn - 1) * (sn + 1);

    /* n^5 */
    seq_t n5 = sn;
    n5 *= sn; /* n^2 */
    n5 *= sn; /* n^3 */
    n5 *= sn; /* n^4 */
    n5 *= sn; /* n^5 */

    /* divide by 2, choosing the factor that is even */
    if ((a_term & 1) == 0) {
        a_term /= 2;
    } else {
        n5 /= 2;
    }

    return a_term * n5;
}
