/*
 * A293503  Least integer k such that k/n^2 > sqrt(2).
 * https://oeis.org/A293503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(sqrt(2) * n^2) = floor(sqrt(2 * n^4)) + 1 (for n >
 *            0), a(0)=0
 * Range:     u8 n <= 13, u16 n <= 215, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* work in 128‑bit to avoid overflow while computing 2 * n^4 */
    __uint128_t nn = n;
    __uint128_t n2 = nn * nn;          /* n^2   */
    __uint128_t n4 = n2 * n2;          /* n^4   */
    __uint128_t m  = n4 * 2;           /* 2 * n^4  */

    /* binary search for floor(sqrt(m)) */
    __uint128_t low  = 0;
    __uint128_t high = m;              /* high is safe for n up to ~10⁴ */
    while (low < high) {
        __uint128_t mid = low + ((high - low + 1) >> 1);
        __uint128_t mid2 = mid * mid;
        if (mid2 <= m)
            low = mid;
        else
            high = mid - 1;
    }

    /* floor(sqrt(m)) is now in low; take the ceiling */
    __uint128_t ans128 = low + 1;

    return (seq_t)ans128;
}
