/*
 * A365204  Centered icositetrachoral numbers.
 * https://oeis.org/A365204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*n^4 - 48*n^3 + 48*n^2 - 24*n + 1
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t once */
    seq_t N = n;

    /* Compute powers of n in seq_t */
    seq_t n2 = N * N;          /* n^2 */
    seq_t n3 = n2 * N;         /* n^3 */
    seq_t n4 = n2 * n2;        /* n^4 */

    /* Compute the polynomial terms */
    seq_t term1 = 24 * n4;     /* 24*n^4 */
    seq_t term2 = 48 * n3;     /* 48*n^3  */
    seq_t term3 = 48 * n2;     /* 48*n^2  */
    seq_t term4 = 24 * N;      /* 24*n    */

    /* Combine terms with alternating signs */
    return term1 - term2 + term3 - term4 + 1;
}
