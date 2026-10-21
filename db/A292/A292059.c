/*
 * A292059  Wiener index of the n X n white bishop graph.
 * https://oeis.org/A292059
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + 2*n - 6*n^2 - 8*n^3 + 6*n^4 + 3*(-1)^n*(-1 - 2*n +
 *            2*n^2))/24
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* values below offset are undefined, return 0 */

    int64_t N   = (int64_t)n;
    int64_t n2  = N * N;            /* n^2 */
    int64_t n3  = n2 * N;           /* n^3 */
    int64_t n4  = n3 * N;           /* n^4 */
    int64_t sign = (n & 1ULL) ? -1 : 1;   /* (-1)^n */

    int64_t term = 3 + 2*N - 6*n2 - 8*n3 + 6*n4
                 + 3*sign * (-1 - 2*N + 2*n2);

    return (seq_t)(term / 24);
}
