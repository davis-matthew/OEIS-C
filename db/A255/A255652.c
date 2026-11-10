/*
 * A255652  Partial sums of A255486.
 * https://oeis.org/A255652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k=0..n} A255486(k), where A255486(k) = number of odd
 *            coefficients of (1+x+x^3+x^4)^k, computed as GF(2) polynomial
 *            bitset powers
 * Range:     u8 n <= 18, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static void A255652_shift_xor(const uint64_t *src, uint64_t *dst, size_t words, unsigned s)
{
    if (s == 0) {
        for (size_t i = 0; i < words; ++i) dst[i] ^= src[i];
        return;
    }
    for (size_t i = 0; i < words; ++i) {
        uint64_t v = src[i];
        dst[i] ^= v << s;
        if (i + 1 < words) dst[i + 1] ^= v >> (64 - s);
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t count = 0;
    static uint64_t *cur = NULL;
    static uint64_t *nxt = NULL;
    static size_t words = 0;

    if (count == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        words = 4;
        cur = (uint64_t *)calloc(words, sizeof(uint64_t));
        nxt = (uint64_t *)calloc(words, sizeof(uint64_t));
        cur[0] = 1ULL;
        cache[0] = 1;
        count = 1;
    }

    while (count <= (size_t)n) {
        size_t k = count;
        if (k >= cache_cap) {
            size_t nc = cache_cap * 2;
            cache = mem_resize(seq_t, cache, nc);
            cache_cap = nc;
        }
        size_t need = (4 * k) / 64 + 1;
        if (need > words) {
            size_t nw = words * 2;
            if (nw < need) nw = need;
            cur = (uint64_t *)realloc(cur, nw * sizeof(uint64_t));
            nxt = (uint64_t *)realloc(nxt, nw * sizeof(uint64_t));
            memset(cur + words, 0, (nw - words) * sizeof(uint64_t));
            memset(nxt + words, 0, (nw - words) * sizeof(uint64_t));
            words = nw;
        }
        memset(nxt, 0, words * sizeof(uint64_t));
        A255652_shift_xor(cur, nxt, words, 0);
        A255652_shift_xor(cur, nxt, words, 1);
        A255652_shift_xor(cur, nxt, words, 3);
        A255652_shift_xor(cur, nxt, words, 4);

        unsigned bits = 0;
        for (size_t i = 0; i < words; ++i)
            bits += (unsigned)__builtin_popcountll(nxt[i]);

        cache[k] = cache[k - 1] + (seq_t)bits;
        uint64_t *t = cur; cur = nxt; nxt = t;
        count++;
    }
    return cache[n];
}
