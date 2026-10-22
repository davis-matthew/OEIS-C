/*
 * A375764  a(n) is the sum of distinct sums of all subsets with two or more
 *          elements of {1, 2, ..., n}.
 * https://oeis.org/A375764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max(0,(n^4+2*n^3+3*n^2+2*n-24)/8)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 429, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* use seq_t for all arithmetic to avoid overflows under the max available size */
    seq_t nn = (seq_t)n;
    seq_t num = nn * nn * nn * nn    /* n^4  */
              + 2 * nn * nn * nn      /* 2*n^3 */
              + 3 * nn * nn           /* 3*n^2 */
              + 2 * nn                /* 2*n  */
              - 24;                   /* constant */

    return num / 8;
}
