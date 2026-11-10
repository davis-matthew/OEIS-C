/*
 * A241666  Gives the value (either 0 or 1) that the trajectory of n under
 *          A241663 eventually reaches.
 * https://oeis.org/A241666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(n) = a(A241663(n)) for n > 1; computed by
 *            iterating A241663 until reaching 0 or 1
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 142 terms through n = 9944 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A241663_f(uint64_t m)
{
    if (m == 1ULL) return 1ULL;
    if ((m & 1ULL) == 0ULL || (m % 3ULL) == 0ULL) return 0ULL;
    uint64_t result = 1ULL;
    for (uint64_t p = 5; p * p <= m; p += 6) {
        for (int k = 0; k < 2; ++k) {
            uint64_t q = p + 2ULL * (uint64_t)k;
            if (m % q == 0ULL) {
                m /= q;
                result *= (q - 4ULL);
                while (m % q == 0ULL) {
                    m /= q;
                    result *= q;
                }
            }
        }
    }
    if (m > 1ULL) result *= (m - 4ULL);
    return result;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    while (m > 1ULL) {
        m = A241663_f(m);
    }
    return (seq_t)m;
}
