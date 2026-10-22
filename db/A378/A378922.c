/*
 * A378922  Number of minimal edge cuts in the n-antiprism graph.
 * https://oeis.org/A378922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^4 - 3*n^3 + 13*n^2 - 12*n + 6)/6
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 337, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    unsigned long long nn = (unsigned long long)n;

    unsigned long long n2 = nn * nn;          /* n^2 */
    unsigned long long n3 = n2 * nn;          /* n^3 */
    unsigned long long n4 = n3 * nn;          /* n^4 */

    unsigned long long term1 = 2ULL * n4;      /* 2*n^4 */
    unsigned long long term2 = 3ULL * n3;      /* 3*n^3 */
    unsigned long long term3 = 13ULL * n2;     /* 13*n^2 */
    unsigned long long term4 = 12ULL * nn;     /* 12*n */

    unsigned long long numerator = term1 - term2 + term3 - term4 + 6ULL;
    unsigned long long res = numerator / 6ULL;

    return (seq_t)res;
}
