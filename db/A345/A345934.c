/*
 * A345934  Ordinal transform of Kempner numbers, A002034.
 * https://oeis.org/A345934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = #{k : 1<=k<=n, A002034(k) = A002034(n)}, where A002034 is
 *            the Kempner function (computed by direct counting)
 * Range:     u8 n <= 15639, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9659 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A345934_kemp(uint64_t m)
{
    if (m <= 1) return 1;
    uint64_t res = 0;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            uint64_t j = 0;
            int v = 0;
            while (v < e) {
                j += p;
                uint64_t t = j;
                while (t % p == 0) { t /= p; ++v; }
            }
            if (j > res) res = j;
        }
    }
    if (m > 1 && m > res) res = m;
    return res;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t target = A345934_kemp(N);
    uint64_t cnt = 0;
    for (uint64_t k = 1; k <= N; ++k) {
        if (A345934_kemp(k) == target) ++cnt;
    }
    return (seq_t)cnt;
}
