/*
 * A241526  Number of different positions in which a square with side length
 *          k, 1 <= k <= n - floor(n/3), can be placed within a bi-symmetric
 *          triangle of 1 X 1 squares of height n.
 * https://oeis.org/A241526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 15*n^2 + 17*n - 6*floor(n/3))/18
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 1058, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* below the offset, undefined */

    seq_t N = (seq_t)n;                 /* promote to the working type */

    seq_t term1 = (seq_t)4 * N * N * N; /* 4*n^3                 */
    seq_t term2 = (seq_t)15 * N * N;    /* 15*n^2                */
    seq_t term3 = (seq_t)17 * N;        /* 17*n                  */
    seq_t term4 = (seq_t)6 * (N / (seq_t)3); /* 6*floor(n/3)   */

    seq_t num = term1 + term2 + term3 - term4; /* numerator   */
    return num / (seq_t)18;                     /* divide by 18 */
}
