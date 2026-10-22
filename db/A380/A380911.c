/*
 * A380911  Triangle read by rows: Row n is the initial segment [1, 2, ..., n]
 *          sorted into lexicographic order defined by the binary
 *          representation of the terms.
 * https://oeis.org/A380911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   Triangular array read by rows; row n is the initial segment
 *            [1..n] sorted lexicographically by the binary representation of
 *            the terms. a(n) is the n‑th element of the flattened array
 * Range:     u8 n <= 9515, u16 n <= 9515, u32 n <= 9515, u64 n <= 9515,
 *            u128 n <= 9515, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

/* Comparator: lexicographic order of binary representations. */
static int cmp_lex(const void *p1, const void *p2) {
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a == *b) return 0;
    uint64_t va = (uint64_t)(*a);
    uint64_t vb = (uint64_t)(*b);
    uint64_t la = 64 - __builtin_clzll(va);
    uint64_t lb = 64 - __builtin_clzll(vb);
    uint64_t minlen = la < lb ? la : lb;
    for (uint64_t i = 0; i < minlen; ++i) {
        int bit_a = (va >> (la - 1 - i)) & 1;
        int bit_b = (vb >> (lb - 1 - i)) & 1;
        if (bit_a != bit_b) return bit_a < bit_b ? -1 : 1;
    }
    return la < lb ? -1 : 1;
}

#ifndef __cplusplus
/* C implementation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t last_row = 0;

/* Ensure cache has space for at least 'needed' elements. */
static void ensure_capacity(size_t needed) {
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined, avoid underflow */
    if ((size_t)n <= cache_len) return cache[n - 1];
    while (cache_len < (size_t)n) {
        idx_t row = last_row + 1;
        seq_t *row_buf = (seq_t *)malloc(row * sizeof(seq_t));
        for (idx_t i = 0; i < row; ++i) row_buf[i] = (seq_t)(i + 1);
        qsort(row_buf, row, sizeof(seq_t), cmp_lex);
        ensure_capacity(cache_len + row);
        memcpy(cache + cache_len, row_buf, row * sizeof(seq_t));
        cache_len += row;
        free(row_buf);
        last_row = row;
    }
    return cache[n - 1];
}

#else
/* C++ implementation using std::vector */
static std::vector<seq_t> cache;
static idx_t last_row = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined */
    if ((size_t)n <= cache.size()) return cache[(size_t)n - 1];
    while (cache.size() < (size_t)n) {
        idx_t row = last_row + 1;
        std::vector<seq_t> tmp(row);
        for (idx_t i = 0; i < row; ++i) tmp[i] = (seq_t)(i + 1);
        std::sort(tmp.begin(), tmp.end(),
                  [](seq_t a, seq_t b) -> bool { return cmp_lex(&a, &b) < 0; });
        cache.insert(cache.end(), tmp.begin(), tmp.end());
        last_row = row;
    }
    return cache[(size_t)n - 1];
}
#endif
