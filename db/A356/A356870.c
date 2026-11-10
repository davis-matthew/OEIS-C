/*
 * A356870  a(n) = (A005132(2*n-1) + A005132(2*n))/4.
 * https://oeis.org/A356870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (A005132(2*n-1) + A005132(2*n))/4, where A005132 is
 *            Recaman's sequence: R(0)=0, R(k)=R(k-1)-k if nonnegative and not
 *            already seen, else R(k-1)+k. Computed incrementally with a
 *            visited bitmap
 * Range:     u8 n <= 50, u16 n <= 8797, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9893 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A356870_cache = NULL;
static idx_t A356870_len = 0;
static unsigned char *A356870_seen = NULL;
static size_t A356870_cap = 0;

static void A356870_mark(size_t v) {
    if (v >= A356870_cap) {
        size_t nc = A356870_cap ? A356870_cap : 1024;
        while (nc <= v) nc *= 2;
        A356870_seen = (unsigned char *)realloc(A356870_seen, nc);
        memset(A356870_seen + A356870_cap, 0, nc - A356870_cap);
        A356870_cap = nc;
    }
    A356870_seen[v] = 1;
}

static bool A356870_isseen(size_t v) {
    return v < A356870_cap && A356870_seen[v];
}

static void A356870_extend(idx_t m) {
    if (m < A356870_len) return;
    if (A356870_cache == NULL) {
        A356870_cache = mem_new(seq_t, (m + 1));
        A356870_cache[0] = 0;
        A356870_len = 1;
        A356870_mark(0);
    } else {
        A356870_cache = mem_resize(seq_t, A356870_cache, (m + 1));
    }
    for (idx_t i = A356870_len; i <= m; i++) {
        seq_t prev = A356870_cache[i - 1];
        seq_t step = (seq_t)i;
        seq_t nv;
        if (prev >= step) {
            seq_t cand = prev - step;
            if (A356870_isseen((size_t)(long long)cand)) nv = prev + step;
            else nv = cand;
        } else {
            nv = prev + step;
        }
        A356870_cache[i] = nv;
        A356870_mark((size_t)(long long)nv);
    }
    A356870_len = m + 1;
}

seq_t a(idx_t n) {
    A356870_extend(2 * n);
    return (A356870_cache[2 * n - 1] + A356870_cache[2 * n]) / 4;
}
