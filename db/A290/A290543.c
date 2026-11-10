/*
 * A290543  Composite numbers n such that A290542(n) >= 2.
 * https://oeis.org/A290543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) is the n-th composite m such that some k in [2,
 *            floor(sqrt(m))] has k^m == k (mod m) (i.e. A290542(m) >= 2)
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A290543_cache = 0;
static size_t A290543_cnt = 0;
static size_t A290543_cap = 0;
static uint64_t A290543_next = 4;

static uint64_t A290543_mulmod(uint64_t x, uint64_t y, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)x * y) % m);
}

static uint64_t A290543_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1)
            r = A290543_mulmod(r, b, m);
        b = A290543_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool A290543_is_composite(uint64_t m)
{
    if (m < 4)
        return false;
    if (m % 2 == 0)
        return true;
    for (uint64_t d = 3; d * d <= m; d += 2)
        if (m % d == 0)
            return true;
    return false;
}

static bool A290543_has_k(uint64_t m)
{
    for (uint64_t k = 2; k * k <= m; ++k) {
        if (A290543_powmod(k, m, m) == k)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    while (A290543_cnt < (size_t)n) {
        uint64_t m = A290543_next++;
        if (!A290543_is_composite(m))
            continue;
        if (!A290543_has_k(m))
            continue;
        if (A290543_cnt == A290543_cap) {
            size_t nc = A290543_cap ? A290543_cap * 2 : 1024;
            uint64_t *p = (uint64_t *)realloc(A290543_cache, nc * sizeof(uint64_t));
            if (!p)
                return 0;
            A290543_cache = p;
            A290543_cap = nc;
        }
        A290543_cache[A290543_cnt++] = m;
    }
    return (seq_t)A290543_cache[n - 1];
}
