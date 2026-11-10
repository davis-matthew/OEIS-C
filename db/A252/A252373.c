/*
 * A252373  Partial sums of A252372, inverse function for A251726.
 * https://oeis.org/A252373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(1) = 0, a(n) = a(n-1) + A252372(n), where A252372(n) = 1 if n
 *            > 1 and gpf(n) < spf(n)^2, else 0 (cumulative count with
 *            memoization)
 * Range:     u8 n <= 726, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 99 terms through n = 7948 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *c_cache = NULL;
static uint64_t c_len = 0;   /* a(1..c_len) known; c_cache[i] = a(i) */
static uint64_t c_cap = 0;

static bool c_qualifies(uint64_t m)
{
    if (m < 2) return false;
    uint64_t spf = 0, gpf = 1, r = m;
    for (uint64_t d = 2; d * d <= r; ++d) {
        if (r % d == 0) {
            if (!spf) spf = d;
            gpf = d;
            while (r % d == 0) r /= d;
        }
    }
    if (r > 1) {
        if (!spf) spf = r;
        gpf = r;
    }
    return gpf < spf * spf;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N < 1) return (seq_t)0;
    if (N > c_len) {
        if (N + 1 > c_cap) {
            uint64_t nc = c_cap ? c_cap : 1024;
            while (nc < N + 1) nc *= 2;
            c_cache = (uint64_t *)realloc(c_cache, nc * sizeof(uint64_t));
            if (!c_cache) exit(1);
            c_cap = nc;
        }
        if (c_len == 0) {
            c_cache[1] = 0;
            c_len = 1;
        }
        for (uint64_t i = c_len + 1; i <= N; ++i)
            c_cache[i] = c_cache[i - 1] + (c_qualifies(i) ? 1 : 0);
        c_len = N;
    }
    return (seq_t)c_cache[N];
}
