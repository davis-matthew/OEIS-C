/*
 * A338235  Numbers k such that k + the sum of the 4th powers of the decimal
 *          digits of k is a square.
 * https://oeis.org/A338235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 165, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9331 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* k is a term iff k + (sum of the 4th powers of its digits) is a square.
   Searched in order in 64 bits (in seq_t it timed out under big), with the
   terms cached. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool is_square_u64(uint64_t x)
{
    uint64_t r = 0;
    for (uint64_t b = (uint64_t)1 << 31; b; b >>= 1)
        if ((r + b) * (r + b) <= x) r += b;
    return r * r == x;
}

seq_t a(idx_t n)
{
    static const uint64_t P4[10] = {0, 1, 16, 81, 256, 625, 1296, 2401, 4096, 6561};
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t last = 0;
    if (n == 0) return (seq_t)0;
    while (len < (size_t)n) {
        uint64_t k = ++last, s = 0;
        for (uint64_t t = k; t; t /= 10) s += P4[t % 10];
        if (!is_square_u64(k + s)) continue;
        if (len == cap) {
            cap = cap ? 2 * cap : 64;
            seq_t *t = mem_resize(seq_t, cache, cap);
            if (!t) abort();
            cache = t;
        }
        cache[len++] = (seq_t)k;
    }
    return cache[n - 1];
}
