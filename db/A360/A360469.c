/*
 * A360469  Only k >= 0 such that, for every odd r > 0, A093179(n) divides the
 *          generalized Fermat number (A007117(n)^r)^(2^k) + 1.
 * https://oeis.org/A360469
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - v2(n+2) where v2(m) is the exponent of 2 in m
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For indices below the offset the behavior is irrelevant. */
    if (n < 3) return 0;

    /* Compute the exponent of 2 dividing n+2 */
    uint64_t m = (uint64_t)n + 2;
    uint64_t exp = 0;
    while ((m & 1ULL) == 0) {
        exp++;
        m >>= 1;
    }

    /* Result fits in seq_t as declared by the build. */
    return (seq_t)n - (seq_t)exp;
}
