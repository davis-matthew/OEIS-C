/*
 * A316261  The number of ways to induce a single pinch on a compact
 *          2-manifold with n handles. (Note: The manifold is embedded in
 *          Euclidean 2-space, and each pinch partitions it into at most two
 *          submanifolds.)
 * https://oeis.org/A316261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 12*n^2 + 73*n + 3*(n+2)*(-1)^n - 6)/24 * +
 *            ((-1)^(2^n - 1) + 1)/2 * For n >= 1 the second term is zero; for
 *            n = 0 it equals 1. * This implementation evaluates the
 *            simplified form: * if n is even: (2*n^3 + 12*n^2 + 76*n)/24 * if
 *            n is odd : (2*n^3 + 12*n^2 + 70*n - 12)/24
 * Range:     u8 n <= 12, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) return (seq_t)1;

    /* Work with a type large enough for intermediate arithmetic */
    uint64_t m = (uint64_t)n;
    uint64_t m2 = m * m;            /* n^2 */
    uint64_t m3 = m2 * m;           /* n^3 */
    uint64_t num;

    if ((n & 1) == 0)        /* n even */
        num = 2ULL * m3 + 12ULL * m2 + 76ULL * m;
    else                     /* n odd */
        num = 2ULL * m3 + 12ULL * m2 + 70ULL * m - 12ULL;

    /* Division by 24 yields the correct integer value */
    uint64_t res = num / 24ULL;
    return (seq_t)res;
}
