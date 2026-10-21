/*
 * A247996  Number of length 1+5 0..n arrays with no disjoint triples in any
 *          consecutive six terms having the same sum.
 * https://oeis.org/A247996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = (2*n^6 + n^5 + 35*n^4 – 5*n^3 + 10*n^2 + 36*n – (15 if
 *            n is odd)) / 2 * for n ≥ 1 (offset 1). * This uses only integer
 *            arithmetic on seq_t, which is either an unsigned * integer type
 *            or an arbitrary‑precision class. No temporary overflow * beyond
 *            seq_t is introduced; arithmetic is perfor...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 35, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* cast index to seq_t */

    /* compute successive powers of n in seq_t */
    seq_t n2 = nn * nn;                  /* n^2   */
    seq_t n3 = n2 * nn;                  /* n^3   */
    seq_t n4 = n3 * nn;                  /* n^4   */
    seq_t n5 = n4 * nn;                  /* n^5   */
    seq_t n6 = n5 * nn;                  /* n^6   */

    /* polynomial numerator */
    seq_t expr = (seq_t)2 * n6            /* 2*n^6 */
              + n5                        /* + n^5 */
              + (seq_t)35 * n4            /* + 35*n^4 */
              - (seq_t)5 * n3             /* - 5*n^3  */
              + (seq_t)10 * n2            /* + 10*n^2 */
              + (seq_t)36 * nn;           /* + 36*n   */

    /* parity adjustment (subtract 15 for odd n) */
    if ((n & 1UL) != 0)
        expr -= (seq_t)15;

    /* divide by 2 */
    expr /= (seq_t)2;

    return expr;
}
