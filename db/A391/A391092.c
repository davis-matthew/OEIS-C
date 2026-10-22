/*
 * A391092  a(n) is the number of 2 X 2 matrices with elements 1..n where at
 *          least one row, at least one column and at least one of the two
 *          diagonals is strictly increasing.
 * https://oeis.org/A391092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(3*n^2 - 2*n - 2)/6
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 303, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the expression gives 0, but we avoid underflow. */
    if (n < 2)
        return (seq_t)0;

    seq_t nn   = (seq_t)n;
    seq_t n1   = nn - (seq_t)1;
    seq_t prod = nn * n1;                      /* n * (n-1) */
    seq_t inner = 3 * nn * nn - 2 * nn - 2;    /* 3*n^2 - 2*n - 2 */
    seq_t val   = prod * inner;                /* n*(n-1)*(3*n^2 - 2*n - 2) */
    return val / 6;                            /* divide by 6 */
}
