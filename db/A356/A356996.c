/*
 * A356996  a(n) = b(n) - b(b(n)) - b(n - b(n)) for n >= 3, where b(n) =
 *          A356989(n).
 * https://oeis.org/A356996
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(n) - b(b(n)) - b(n - b(n)) for n >= 3, where b(n) =
 *            A356989(n) = n - b(b(b(n - b(b(b(b(n-1))))))), b(1) = 1
 * Range:     u8 n <= 125, u16 n <= 125, u32 n <= 125, u64 n <= 125,
 *            u128 n <= 125, big any n
 * Validated: partial, pass, 123 terms through n = 125 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A356996_cache = NULL;
static idx_t A356996_cap = 0;
static idx_t A356996_len = 0;

static inline seq_t A356996_get(idx_t k) { return A356996_cache[k - 1]; }

static void A356996_fill(idx_t n) {
    if (n <= A356996_len) return;
    if (n > A356996_cap) {
        idx_t nc = A356996_cap ? A356996_cap : 16;
        while (nc < n) nc <<= 1;
        seq_t *tmp = mem_resize(seq_t, A356996_cache, nc);
        if (!tmp) abort();
        A356996_cache = tmp;
        A356996_cap = nc;
    }
    for (idx_t i = A356996_len + 1; i <= n; ++i) {
        if (i == 1) { A356996_cache[0] = (seq_t)1; continue; }
        seq_t v1 = A356996_get(i - 1);
        seq_t v2 = A356996_get((idx_t)v1);
        seq_t v3 = A356996_get((idx_t)v2);
        seq_t v4 = A356996_get((idx_t)v3);
        idx_t t = i - (idx_t)v4;
        seq_t w1 = A356996_get(t);
        seq_t w2 = A356996_get((idx_t)w1);
        seq_t w3 = A356996_get((idx_t)w2);
        A356996_cache[i - 1] = (seq_t)i - w3;
    }
    A356996_len = n;
}

seq_t a(idx_t n) {
    if (n < 3) return (seq_t)0;
    A356996_fill(n);
    seq_t bn = A356996_get(n);
    seq_t r1 = A356996_get((idx_t)bn);
    seq_t r2 = A356996_get(n - (idx_t)bn);
    return bn - r1 - r2;
}
