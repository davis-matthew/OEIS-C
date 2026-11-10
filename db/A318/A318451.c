/*
 * A318451  The 2-adic valuation of A318450.
 * https://oeis.org/A318451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{p^e || n, p odd prime} (2*e - popcount(e));
 *            additive, a(2^e) = 0 (equivalently A046645(A000265(n)))
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9706 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;
    uint64_t m = (uint64_t)n;
    while ((m & 1) == 0) m >>= 1;
    uint64_t result = 0;
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            uint64_t e = 0;
            while (m % d == 0) {
                m /= d;
                ++e;
            }
            uint64_t pop = 0, t = e;
            while (t) { t &= t - 1; ++pop; }
            result += 2 * e - pop;
        }
    }
    if (m > 1) result += 1;
    return (seq_t)result;
}
