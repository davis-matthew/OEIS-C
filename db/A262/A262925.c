/*
 * A262925  Sum of n consecutive fourth powers starting with n^4.
 * https://oeis.org/A262925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(-1+70*n^2-225*n^3+186*n^4)/30
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 57, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for the offset */
    if (n == 0) return (seq_t)0;

    seq_t N   = (seq_t)n;          /* n as seq_t */
    seq_t n2  = N * N;             /* n^2 */
    seq_t n3  = n2 * N;            /* n^3 */
    seq_t n4  = n3 * N;            /* n^4 */

    seq_t t1  = (seq_t)70 * n2;    /* 70*n^2 */
    seq_t t2  = (seq_t)225 * n3;   /* 225*n^3 */
    seq_t t3  = (seq_t)186 * n4;   /* 186*n^4 */

    /* expr = -1 + 70*n^2 - 225*n^3 + 186*n^4 */
    seq_t expr = t1 + t3;           /* add positive terms */
    expr -= t2;                    /* subtract 225*n^3 */
    expr -= (seq_t)1;              /* subtract 1 */

    seq_t num = N * expr;          /* n * expr */
    return num / (seq_t)30;        /* division by 30, divisible exactly */
}
