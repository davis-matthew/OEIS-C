/*
 * A378709  Circumference of the n-Pell graph.
 * https://oeis.org/A378709
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = Pell(n+1) - (n even ? 1 : 0)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute Pell number P_{n+1} with recurrence P_0 = 0, P_1 = 1 */
    seq_t p0 = (seq_t)0;
    seq_t p1 = (seq_t)1;
    idx_t m = n + (idx_t)1;          /* we need P_{n+1} */

    if (m == 0) {
        /* n would be -1, not expected; return 0 to keep type safety */
        return (seq_t)0;
    }

    for (idx_t k = (idx_t)2; k <= m; ++k) {
        seq_t p2 = p1 * (seq_t)2 + p0;
        p0 = p1;
        p1 = p2;
    }

    seq_t result = p1;
    if ((n & (idx_t)1) == 0) {       /* n is even: subtract 1 */
        result = result - (seq_t)1;
    }
    return result;
}
