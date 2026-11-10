/*
 * A241665  Number of iterations of A241663 needed to reach either 0 or 1.
 * https://oeis.org/A241665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of applications of A241663 (do-while, at least
 *            one) to n until the value is 0 or 1; A241663(n)=1 if n=1, 0 if
 *            2|n or 3|n, else prod p^(e-1)*(p-4)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 7966 (2026-11-07)
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
        if (m % p == 0ULL) {
            uint64_t pw = 1ULL;
            m /= p;
            while (m % p == 0ULL) {
                m /= p;
                pw *= p;
            }
            result *= pw * (p - 4ULL);
        }
        uint64_t p2 = p + 2;
        if (m % p2 == 0ULL) {
            uint64_t pw = 1ULL;
            m /= p2;
            while (m % p2 == 0ULL) {
                m /= p2;
                pw *= p2;
            }
            result *= pw * (p2 - 4ULL);
        }
    }
    if (m > 1ULL) result *= (m - 4ULL);
    return result;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    unsigned long long count = 0;
    do {
        m = A241663_f(m);
        count++;
    } while (m > 1ULL);
    return (seq_t)count;
}
