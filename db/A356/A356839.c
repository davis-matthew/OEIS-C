/*
 * A356839  a(n) = A005132(2*n) + A005132(2*n+1).
 * https://oeis.org/A356839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = R(2n) + R(2n+1), where R = A005132 (Recaman): R(0)=0,
 *            R(k) = R(k-1)-k if >=0 and not already seen, else R(k-1)+k
 * Range:     u8 n <= 0, u16 n <= 4777, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 75 terms through n = 9901 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A005132_cache = NULL;
static idx_t A005132_len = 0;
static idx_t A005132_cap = 0;
static unsigned char *A005132_seen = NULL;
static size_t A005132_seen_cap = 0;

static void A005132_mark(seq_t v) {
    size_t x = (size_t)(unsigned long long)v;
    if (x >= A005132_seen_cap) {
        size_t nc = A005132_seen_cap ? A005132_seen_cap : 1024;
        while (nc <= x) nc *= 2;
        A005132_seen = (unsigned char *)realloc(A005132_seen, nc);
        memset(A005132_seen + A005132_seen_cap, 0, nc - A005132_seen_cap);
        A005132_seen_cap = nc;
    }
    A005132_seen[x] = 1;
}

static bool A005132_isseen(seq_t v) {
    if (v >= (seq_t)A005132_seen_cap) return false;
    return A005132_seen[(size_t)(unsigned long long)v] != 0;
}

static void A005132_extend(idx_t m) {
    if (m < A005132_len) return;
    if (m >= A005132_cap) {
        idx_t nc = A005132_cap ? A005132_cap : 1024;
        while (nc <= m) nc *= 2;
        if (A005132_cache == NULL) A005132_cache = mem_new(seq_t, nc);
        else A005132_cache = mem_resize(seq_t, A005132_cache, nc);
        A005132_cap = nc;
    }
    if (A005132_len == 0) {
        A005132_cache[0] = 0;
        A005132_mark((seq_t)0);
        A005132_len = 1;
    }
    for (idx_t i = A005132_len; i <= m; i++) {
        seq_t prev = A005132_cache[i - 1];
        seq_t k = (seq_t)i;
        seq_t v;
        if (prev >= k && !A005132_isseen(prev - k)) v = prev - k;
        else v = prev + k;
        A005132_cache[i] = v;
        A005132_mark(v);
    }
    A005132_len = m + 1;
}

seq_t a(idx_t n) {
    A005132_extend(2 * n + 1);
    return A005132_cache[2 * n] + A005132_cache[2 * n + 1];
}
