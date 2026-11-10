/*
 * A339914  Ordinal transform of sequence b(1) = 1, b(n) = A143771(n) for n >
 *          1.
 * https://oeis.org/A339914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of j <= n with b(j) = b(n), where b(1) = 1, b(n) =
 *            A143771(n) for n > 1 (ordinal transform)
 * Range:     u8 n <= 765, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 111 terms through n = 9980 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A339914_gcd(uint64_t x, uint64_t y)
{
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

/* A143771(n): gcd of k + n/k over divisors k of n */
static uint64_t A339914_A143771(uint64_t n)
{
    uint64_t g = 0;
    for (uint64_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            uint64_t v = i + n / i;
            g = g ? A339914_gcd(g, v) : v;
        }
    }
    return g;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint32_t *cnt = (uint32_t *)calloc((size_t)n + 3, sizeof(uint32_t));
    if (!cnt) return 0;
    uint64_t last = 1;
    uint32_t c = 0;
    for (uint64_t k = 1; k <= n; ++k) {
        uint64_t b = (k == 1) ? 1 : A339914_A143771(k);
        c = ++cnt[b];
        last = b;
    }
    (void)last;
    free(cnt);
    return (seq_t)c;
}
