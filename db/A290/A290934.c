/*
 * A290934  a(n) equals the sum of the squares of the digits of A254958(n).
 * https://oeis.org/A290934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of squares of the decimal digits of A254958(n);
 *            A254958 = zeroless numbers whose digit-square sum is a perfect
 *            square (found by a cached search)
 * Range:     u8 n <= 232, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 79 terms through n = 997 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A290934_cache = NULL;
static size_t A290934_len = 0;
static size_t A290934_cap = 0;
static uint64_t A290934_next = 1;

static bool A290934_is_sq(uint64_t s)
{
    uint64_t r = 0;
    while (r * r < s)
        r++;
    return r * r == s;
}

seq_t a(idx_t n)
{
    size_t want = (size_t)n;
    while (A290934_len < want) {
        uint64_t x = A290934_next++;
        uint64_t t = x, s = 0;
        bool zero = false;
        while (t > 0) {
            uint64_t d = t % 10;
            if (d == 0) { zero = true; break; }
            s += d * d;
            t /= 10;
        }
        if (zero)
            continue;
        if (A290934_is_sq(s)) {
            if (A290934_len == A290934_cap) {
                size_t nc = A290934_cap ? A290934_cap * 2 : 1024;
                A290934_cache = (uint64_t *)realloc(A290934_cache, nc * sizeof(uint64_t));
                A290934_cap = nc;
            }
            A290934_cache[A290934_len++] = s;
        }
    }
    return (seq_t)A290934_cache[want - 1];
}
