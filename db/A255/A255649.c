/*
 * A255649  Partial sums of A134661.
 * https://oeis.org/A255649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k=0..n} A134661(k), where A134661(k) = number of odd
 *            coefficients in (1 + x + x^3)^k
 * Range:     u8 n <= 22, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static inline seq_t A134661_popcount(uint64_t v) {
    return (seq_t)__builtin_popcountll(v);
}

static seq_t A134661_a(idx_t n) {
    static seq_t *counts = NULL;
    static size_t counts_cap = 0;
    static idx_t counts_len = 0;

    static uint64_t *poly_cur = NULL;
    static size_t poly_words = 0;
    static idx_t cur_n = 0;

    if (!poly_cur) {
        poly_words = 1;
        poly_cur = (uint64_t*)calloc(poly_words, sizeof(uint64_t));
        if (!poly_cur) return 0;
        poly_cur[0] = 1ULL;
        cur_n = 0;

        counts_cap = 1;
        counts = mem_new(seq_t, counts_cap);
        if (!counts) return 0;
        counts[0] = 1;
        counts_len = 1;
    }

    if (n < counts_len)
        return counts[n];

    if (n >= counts_cap) {
        size_t new_cap = (counts_cap + 1) * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, counts, new_cap);
        if (!tmp) return 0;
        counts = tmp;
        counts_cap = new_cap;
    }

    for (idx_t i = cur_n + 1; i <= n; ++i) {
        idx_t deg_new = (idx_t)3 * i;
        size_t words_new = (size_t)((deg_new + 1 + 63) / 64);

        uint64_t *poly_next = (uint64_t*)calloc(words_new, sizeof(uint64_t));
        if (!poly_next) return 0;

        for (size_t j = 0; j < poly_words; ++j)
            poly_next[j] ^= poly_cur[j];

        for (size_t j = 0; j < poly_words; ++j) {
            uint64_t v = poly_cur[j];
            poly_next[j] ^= v << 1;
            if (j + 1 < words_new)
                poly_next[j + 1] ^= v >> 63;
        }

        for (size_t j = 0; j < poly_words; ++j) {
            uint64_t v = poly_cur[j];
            poly_next[j] ^= v << 3;
            if (j + 1 < words_new)
                poly_next[j + 1] ^= v >> 61;
        }

        seq_t cnt = 0;
        for (size_t j = 0; j < words_new; ++j)
            cnt += A134661_popcount(poly_next[j]);

        counts[i] = cnt;
        counts_len = (idx_t)(i + 1);

        free(poly_cur);
        poly_cur = poly_next;
        poly_words = words_new;
        cur_n = i;
    }

    return counts[n];
}

seq_t a(idx_t n) {
    static seq_t *ps = NULL;
    static size_t ps_cap = 0;
    static idx_t ps_len = 0;

    if (n < ps_len)
        return ps[n];

    if ((size_t)n + 1 > ps_cap) {
        size_t new_cap = ps_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        if (new_cap < 16) new_cap = 16;
        seq_t *tmp;
        if (ps == NULL)
            tmp = mem_new(seq_t, new_cap);
        else
            tmp = mem_resize(seq_t, ps, new_cap);
        if (!tmp) return 0;
        ps = tmp;
        ps_cap = new_cap;
    }

    for (idx_t k = ps_len; k <= n; ++k) {
        seq_t prev = (k == 0) ? (seq_t)0 : ps[k - 1];
        ps[k] = prev + A134661_a(k);
        ps_len = k + 1;
    }
    return ps[n];
}
