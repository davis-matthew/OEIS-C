/*
 * A323080  Lexicographically earliest such sequence a that for all i, j, a(i)
 *          = a(j) => f(i) = f(j), where f(1)=0, and f(n) = A252463(n) for n >
 *          1.
 * https://oeis.org/A323080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = rank of first occurrence of f(n) among f(1..n), f(1)=0,
 *            f(n)=A252463(n) (restricted growth string); computed by a
 *            memoized sieve-like scan
 * Range:     u8 n <= 410, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9520 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool A323080_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A323080_prevp(uint64_t n)
{
    uint64_t p = n - 1;
    while (!A323080_isp(p)) --p;
    return p;
}

static uint64_t A323080_f(uint64_t n)
{
    if (n == 1) return 0;
    if ((n & 1) == 0) return n >> 1;
    uint64_t m = n, result = 1;
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t base = A323080_prevp(p);
            while (m % p == 0) {
                m /= p;
                result *= base;
            }
        }
    }
    if (m > 1) result *= A323080_prevp(m);
    return result;
}

static uint64_t *A323080_tab = NULL;
static uint64_t A323080_done = 0;

seq_t a(idx_t n)
{
    if ((uint64_t)n > A323080_done) {
        uint64_t m = (uint64_t)n;
        if (m < 2 * A323080_done) m = 2 * A323080_done;
        if (m < 64) m = 64;
        uint64_t *ids = (uint64_t *)calloc((size_t)m + 2, sizeof(uint64_t));
        uint64_t *tab = (uint64_t *)malloc(((size_t)m + 1) * sizeof(uint64_t));
        uint64_t cnt = 0;
        for (uint64_t i = 1; i <= m; ++i) {
            uint64_t v = A323080_f(i);
            if (ids[v] == 0) ids[v] = ++cnt;
            tab[i] = ids[v];
        }
        free(ids);
        free(A323080_tab);
        A323080_tab = tab;
        A323080_done = m;
    }
    return (seq_t)A323080_tab[(uint64_t)n];
}
