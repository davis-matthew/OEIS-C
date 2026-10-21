/*
 * A276914  Subsequence of triangular numbers obtained by adding a square and
 *          two smaller triangles, a(n) = n^2 + 2*A000217(A052928(n)).
 * https://oeis.org/A276914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n + (-1)^n)
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1U)          /* odd n: n*(2*n - 1) */
    {
        return (seq_t)n * (((seq_t)n * 2) - 1);
    }
    else                 /* even n: n*(2*n + 1) */
    {
        return (seq_t)n * (((seq_t)n * 2) + 1);
    }
}
