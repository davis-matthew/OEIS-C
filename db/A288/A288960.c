/*
 * A288960  Number of 6-cycles in the n X n rook graph.
 * https://oeis.org/A288960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n-2)*n^2*(n+2)*(n^2+2*n-11)/6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n <= 2 the product contains zero */
    if (n <= 2) {
        return 0;
    }

    seq_t nn      = (seq_t)n;
    seq_t n_1     = nn - 1;      /* n-1 */
    seq_t n_2     = nn - 2;      /* n-2 */
    seq_t n_plus2 = nn + 2;      /* n+2 */

    /* n^2 + 2*n - 11 */
    seq_t n2_plus = nn * nn + 2 * nn - 11;

    /* Compute the product step by step to avoid overflow on 64‑bit build */
    seq_t temp = n_1 * n_2;   /* (n-1)(n-2)   */
    temp = temp * nn;        /* * n         */
    temp = temp * nn;        /* * n again   */
    temp = temp * n_plus2;   /* * (n+2)     */
    temp = temp * n2_plus;   /* * (n^2+2n-11) */
    temp = temp / 6;         /* divide by 6 */

    return temp;
}
