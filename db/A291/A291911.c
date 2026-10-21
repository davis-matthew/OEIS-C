/*
 * A291911  Number of 5-cycles in the n X n rook complement graph.
 * https://oeis.org/A291911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 2)^2*(n - 1)^2*n^2*(4 + 2*n + 3*n^2 - 4*n^3 +
 *            n^4)/10
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the product contains a zero factor, giving 0. */
    if (n < 3)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t n1 = nn - 1;   /* n-1 (safe, n >= 3) */
    seq_t n2 = nn - 2;   /* n-2 (safe, n >= 3) */

    /* Compute (n-1)^2 * (n-2)^2 * n^2 */
    seq_t part = n1 * n1;
    part = part * n2 * n2;
    part = part * nn * nn;

    /* Compute the polynomial 4 + 2*n + 3*n^2 - 4*n^3 + n^4 */
    seq_t poly = 4
                 + 2 * nn
                 + 3 * nn * nn
                 - 4 * nn * nn * nn
                 + nn * nn * nn * nn;

    /* Final result; the division by 10 yields an integer for all n >= 0. */
    return part * poly / 10;
}
