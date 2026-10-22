/*
 * A366304  Square array A(n, k), n, k >= 0, read and filled by upwards
 *          antidiagonals the greedy way with distinct nonnegative integers
 *          such that the binary expansions of any two distinct terms in the
 *          same row or column have no common 1's.
 * https://oeis.org/A366304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = value at index n obtained by greedy filling of the square
 *            array with bit‑disjoint rows and columns, read by upwards
 *            antidiagonals
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   antidiagonal_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/antidiagonal_coords.h"

static idx_t cache_len = 0;
static seq_t *cache = NULL;

static size_t rowSize = 0;
static seq_t *rowMask = NULL;
static size_t colSize = 0;
static seq_t *colMask = NULL;

typedef struct {
    size_t cap;
    size_t size;
    seq_t *table;
    uint8_t *used;
} HashSet;

static HashSet usedSet = {0,0,NULL,NULL};

static void hashset_init(void) {
    usedSet.cap = 8;
    usedSet.size = 0;
    usedSet.table = (seq_t*)malloc(usedSet.cap * sizeof(seq_t));
    usedSet.used = (uint8_t*)malloc(usedSet.cap * sizeof(uint8_t));
    memset(usedSet.used, 0, usedSet.cap);
}

static inline size_t hash_val(seq_t v) {
    return (size_t)(v);
}

static void hashset_grow(void) {
    size_t newCap = usedSet.cap * 2;
    seq_t *newTable = (seq_t*)malloc(newCap * sizeof(seq_t));
    uint8_t *newUsed = (uint8_t*)malloc(newCap * sizeof(uint8_t));
    memset(newUsed, 0, newCap);
    size_t mask = newCap - 1;
    for (size_t i = 0; i < usedSet.cap; ++i) {
        if (usedSet.used[i]) {
            seq_t val = usedSet.table[i];
            size_t h = (hash_val(val) & mask);
            while (newUsed[h]) h = (h + 1) & mask;
            newTable[h] = val;
            newUsed[h] = 1;
        }
    }
    free(usedSet.table);
    free(usedSet.used);
    usedSet.table = newTable;
    usedSet.used = newUsed;
    usedSet.cap = newCap;
}

static int hashset_contains(seq_t val) {
    if (!usedSet.cap) hashset_init();
    size_t h = hash_val(val) & (usedSet.cap - 1);
    while (usedSet.used[h]) {
        if (usedSet.table[h] == val) return 1;
        h = (h + 1) & (usedSet.cap - 1);
    }
    return 0;
}

static void hashset_insert(seq_t val) {
    if (!usedSet.cap) hashset_init();
    if (usedSet.size * 3 >= usedSet.cap * 2) hashset_grow();
    size_t h = hash_val(val) & (usedSet.cap - 1);
    while (usedSet.used[h]) h = (h + 1) & (usedSet.cap - 1);
    usedSet.table[h] = val;
    usedSet.used[h] = 1;
    usedSet.size++;
}

static void ensure_row(idx_t r) {
    if ((size_t)r >= rowSize) {
        size_t newSize = rowSize ? rowSize * 2 : 1;
        while (newSize <= (size_t)r) newSize *= 2;
        rowMask = (seq_t*)realloc(rowMask, newSize * sizeof(seq_t));
        memset(rowMask + rowSize, 0, (newSize - rowSize) * sizeof(seq_t));
        rowSize = newSize;
    }
}

static void ensure_col(idx_t c) {
    if ((size_t)c >= colSize) {
        size_t newSize = colSize ? colSize * 2 : 1;
        while (newSize <= (size_t)c) newSize *= 2;
        colMask = (seq_t*)realloc(colMask, newSize * sizeof(seq_t));
        memset(colMask + colSize, 0, (newSize - colSize) * sizeof(seq_t));
        colSize = newSize;
    }
}

static void compute_to(idx_t n) {
    if (n < cache_len) return;
    size_t newLen = (size_t)n + 1;
    cache = (seq_t*)realloc(cache, newLen * sizeof(seq_t));
    for (size_t idx = cache_len; idx < newLen; ++idx) {
        idx_t i = (idx_t)idx;
        idx_t row, col;
        antidiagonal_coords(i, &row, &col);
        ensure_row(row);
        ensure_col(col);
        seq_t cand = 0;
        while (1) {
            if (!hashset_contains(cand)) {
                if ((rowMask[row] & cand) == 0 && (colMask[col] & cand) == 0) break;
            }
            cand = cand + 1;
        }
        cache[idx] = cand;
        rowMask[row] = rowMask[row] | cand;
        colMask[col] = colMask[col] | cand;
        hashset_insert(cand);
    }
    cache_len = newLen;
}

seq_t a(idx_t n) {
    compute_to(n);
    return cache[n];
}
