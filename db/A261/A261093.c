/*
 * A261093  a(n) = number of nonzero terms of A219640 <= n.
 * https://oeis.org/A261093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (number of distinct values of k - ZeckendorfPopcount(k)
 *            that are <= n) - 1; computed by scanning k upward (k - popcount
 *            is nondecreasing)
 * Range:     u8 n <= 410, u16 n <= 6765, u32 n <= 6765, u64 n <= 6765,
 *            u128 n <= 6765, big any n
 * Validated: standard, pass, 105 terms through n = 6708 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A261093_zpop(uint64_t k, const uint64_t *f, int cnt)
{
    unsigned c = 0;
    for (int i = cnt - 1; i >= 0 && k > 0; --i) {
        if (f[i] <= k) {
            ++c;
            k -= f[i];
        }
    }
    return c;
}

seq_t a(idx_t n)
{
    static uint64_t f[100];
    static int cnt = 0;
    if (cnt == 0) {
        f[0] = 1;
        f[1] = 2;
        cnt = 2;
        while (cnt < 100) {
            uint64_t nx = f[cnt - 1] + f[cnt - 2];
            if (nx < f[cnt - 1]) break;
            f[cnt++] = nx;
        }
    }
    uint64_t count = 0;
    int have = 0;
    uint64_t last = 0;
    for (uint64_t k = 0;; ++k) {
        uint64_t m = k - A261093_zpop(k, f, cnt);
        if (m > n) break;
        if (!have || m > last) {
            ++count;
            last = m;
            have = 1;
        }
    }
    return (seq_t)(count - 1);
}
