/*
 * A278670  Number of n X 2 0..1 arrays with rows in nondecreasing
 *          lexicographic order and columns in nonincreasing lexicographic
 *          order, but with exactly one mistake.
 * https://oeis.org/A278670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/720)*n^6 + (1/48)*n^5 + (23/144)*n^4 + (19/48)*n^3 +
 *            (61/180)*n^2 + (1/12)*n (OEIS A278670)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; for n == 0 return 0 as a safe default */
    if (n == 0)
        return (seq_t)0;

    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;          /* n^2   */
    seq_t n3 = n2 * N;         /* n^3   */
    seq_t n4 = n3 * N;         /* n^4   */
    seq_t n5 = n4 * N;         /* n^5   */
    seq_t n6 = n5 * N;         /* n^6   */

    seq_t num = 0;
    num += n6;                     /* 1/720 part   */
    num += (seq_t)15 * n5;          /* 15/720 = 1/48 part  */
    num += (seq_t)115 * n4;         /* 115/720 = 23/144 part  */
    num += (seq_t)285 * n3;         /* 285/720 = 19/48 part  */
    num += (seq_t)244 * n2;         /* 244/720 = 61/180 part */
    num += (seq_t)60 * N;           /* 60/720 = 1/12 part  */

    return num / (seq_t)720;
}
