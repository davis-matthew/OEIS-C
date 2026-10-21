/*
 * A272000  Coinage sequence: a(n) = A018227(n)-7.
 * https://oeis.org/A272000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 9*n^2 + 26*n - 30)/6 for n even, a(n) = (n^3 +
 *            9*n^2 + 29*n - 21)/6 for n odd
 * Range:     u8 n <= 8, u16 n <= 70, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    uint64_t nn = (uint64_t)n;
    uint64_t n2 = nn * nn;
    uint64_t n3 = n2 * nn;
    uint64_t num;
    if ((n & 1) == 0) {
        num = n3 + 9ULL * n2 + 26ULL * nn - 30ULL;
    } else {
        num = n3 + 9ULL * n2 + 29ULL * nn - 21ULL;
    }
    return (seq_t)(num / 6ULL);
}
