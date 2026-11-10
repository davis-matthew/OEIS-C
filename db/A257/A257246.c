/*
 * A257246  Even bisection of A257244: a(n) = A257244(2n).
 * https://oeis.org/A257246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lpf(b(2n)), where b = A256393: b(1)=2,
 *            b(2k)=b(2k-1)+gpf(b(2k-1)), b(2k+1)=b(2k)+lpf(b(2k)); lpf =
 *            least prime factor
 * Range:     u8 n <= 282, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Validated: standard, pass, 97 terms through n = 2025 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A257246_lpf(uint64_t n)
{
    if (n % 2 == 0) return 2;
    for (uint64_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;
}

static uint64_t A257246_gpf(uint64_t n)
{
    uint64_t best = 1;
    while (n % 2 == 0) { best = 2; n /= 2; }
    for (uint64_t i = 3; i <= n / i; i += 2) {
        while (n % i == 0) { best = i; n /= i; }
    }
    if (n > 1) best = n;
    return best;
}

seq_t a(idx_t n)
{
    static uint64_t *cache = NULL;   /* cache[k-1] = A256393(k) */
    static size_t len = 0, cap = 0;

    if (n == 0) return (seq_t)0;
    size_t need = (size_t)(2 * n);

    if (cache == NULL) {
        cap = 1024;
        cache = (uint64_t *)malloc(cap * sizeof(uint64_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = 2;
        len = 1;
    }
    if (need > cap) {
        while (cap < need) cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(cache, cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
    }
    for (size_t i = len + 1; i <= need; ++i) {
        uint64_t prev = cache[i - 2];
        uint64_t f = (i % 2 == 0) ? A257246_gpf(prev) : A257246_lpf(prev);
        cache[i - 1] = prev + f;
    }
    if (need > len) len = need;

    return (seq_t)A257246_lpf(cache[need - 1]);
}
