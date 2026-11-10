/*
 * A364731  a(n) is the index of the first occurrence of n in A364443, or -1
 *          if n doesn't occur.
 * https://oeis.org/A364731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least m with A364443(m) = n (A364443(m) = #{Loeschian k :
 *            m^2 < k < (m+1)^2}), or -1 if none; computed by a segmented
 *            sieve of x^2+x*y+y^2 values plus a linear search with a cutoff
 * Range:     i8 n <= 54, i16 n <= 9218, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9280 (2026-11-10)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/isqrt.h"

static uint32_t *A364731_cnt = NULL;
static idx_t A364731_done = 0;
static idx_t A364731_cap = 0;

static uint64_t A364731_val(uint64_t x, uint64_t y) { return x * x + x * y + y * y; }

static void A364731_extend(void)
{
    idx_t m0 = A364731_done;
    idx_t m1 = m0 + 256;
    uint64_t L = (uint64_t)m0 * m0;
    uint64_t H = (uint64_t)m1 * m1;
    size_t words = (size_t)((H - L) / 64 + 2);
    uint64_t *bits = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!bits) abort();

    if (m1 > A364731_cap) {
        idx_t nc = A364731_cap ? A364731_cap * 2 : 1024;
        while (nc < m1) nc *= 2;
        A364731_cnt = (uint32_t *)realloc(A364731_cnt, (size_t)nc * sizeof(uint32_t));
        if (!A364731_cnt) abort();
        A364731_cap = nc;
    }

    for (uint64_t x = (uint64_t)isqrt((idx_t)(L / 3)); x * x < H; ++x) {
        if (3 * x * x < L) continue;
        uint64_t y = 0;
        if (x * x < L) {
            uint64_t s = (uint64_t)isqrt((idx_t)(4 * L - 3 * x * x));
            y = (s > x) ? (s - x) / 2 : 0;
            while (y <= x && A364731_val(x, y) < L) ++y;
            while (y > 0 && A364731_val(x, y - 1) >= L) --y;
        }
        for (; y <= x; ++y) {
            uint64_t v = A364731_val(x, y);
            if (v >= H) break;
            uint64_t b = v - L;
            bits[b >> 6] |= 1ULL << (b & 63);
        }
    }

    for (idx_t m = m0; m < m1; ++m) {
        uint64_t lo = (uint64_t)m * m + 1;
        uint64_t hi = (uint64_t)(m + 1) * (m + 1) - 1;
        uint32_t c = 0;
        if (lo <= hi) {
            uint64_t a = lo - L, b = hi - L;
            uint64_t wa = a >> 6, wb = b >> 6;
            for (uint64_t w = wa; w <= wb; ++w) {
                uint64_t word = bits[w];
                if (w == wa) word &= ~0ULL << (a & 63);
                if (w == wb) {
                    unsigned rb = (unsigned)(b & 63);
                    if (rb != 63) word &= (1ULL << (rb + 1)) - 1;
                }
                c += (uint32_t)__builtin_popcountll(word);
            }
        }
        A364731_cnt[m] = c;
    }
    free(bits);
    A364731_done = m1;
}

seq_t a(idx_t n)
{
    uint64_t margin = (uint64_t)n / 8 + 10;
    for (idx_t m = 0;; ++m) {
        while (m >= A364731_done) A364731_extend();
        uint64_t c = A364731_cnt[m];
        if (c == (uint64_t)n) return (seq_t)m;
        if (c > (uint64_t)n + margin) return (seq_t)(-1);
    }
}
