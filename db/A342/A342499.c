/*
 * A342499  Number of integer partitions of n with strictly decreasing first
 *          quotients.
 * https://oeis.org/A342499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer partitions of n with strictly
 *            decreasing first quotients
 * Range:     u8 n <= 41, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   hash_keys
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/hash_keys.h"

/* ---- Simple hash map for memoisation of (rem,prev,curr) -> seq_t -------- */

typedef struct {
    idx_t rem, prev, curr;
    seq_t val;
} MapEntry;

static const idx_t EMPTY_KEY = (idx_t)-1;
static const size_t MAP_CAP = 1 << 20;      /* 1,048,576 entries – ample for n<=10000 */
static MapEntry *map_entries = NULL;
static size_t map_size = 0;

/* Initialise the hash table */
static void init_map(void) {
    if (map_entries != NULL) return;
    map_entries = (MapEntry *)malloc(MAP_CAP * sizeof(MapEntry));
    for (size_t i = 0; i < MAP_CAP; ++i) map_entries[i].rem = EMPTY_KEY;
}

static inline size_t hash_index(idx_t key) {
    return (size_t)(key & (MAP_CAP - 1));
}

/* Find in map – return true and store value in *out if present */
static bool map_find(idx_t rem, idx_t prev, idx_t curr, seq_t *out) {
    size_t h = hash_index(hash_keys(rem, prev, curr));
    for (;;) {
        if (map_entries[h].rem == EMPTY_KEY) return false;
        if (map_entries[h].rem == rem &&
            map_entries[h].prev == prev &&
            map_entries[h].curr == curr) {
            *out = map_entries[h].val;
            return true;
        }
        h = (h + 1) & (MAP_CAP - 1);
    }
}

/* Insert into map */
static void map_insert(idx_t rem, idx_t prev, idx_t curr, seq_t val) {
    size_t h = hash_index(hash_keys(rem, prev, curr));
    for (;;) {
        if (map_entries[h].rem == EMPTY_KEY) {
            map_entries[h].rem = rem;
            map_entries[h].prev = prev;
            map_entries[h].curr = curr;
            map_entries[h].val = val;
            ++map_size;
            return;
        }
        h = (h + 1) & (MAP_CAP - 1);
    }
}

/* Recursive counting of partitions that finish the remaining sum `rem`,
   given the last two parts are prev (before curr) and curr.              */
static seq_t count_rem(idx_t rem, idx_t prev, idx_t curr) {
    if (rem == 0) return (seq_t)1;
    seq_t cached;
    if (map_find(rem, prev, curr, &cached)) return cached;

    idx_t limit;
    if (prev == 0) {                     /* first step – no earlier ratio */
        limit = curr;
    } else {
        idx_t sq = curr * curr;
        limit = (sq - 1) / prev;         /* inequality: next * prev < curr^2 */
        if (limit > curr) limit = curr;
    }
    if (limit > rem) limit = rem;

    seq_t total = 0;
    for (idx_t next = 1; next <= limit; ++next) {
        total += count_rem(rem - next, curr, next);
    }
    map_insert(rem, prev, curr, total);
    return total;
}

/* Entry point required by the build system */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    init_map();          /* initialise memoisation table once */

    seq_t total = 0;
    for (idx_t first = 1; first <= n; ++first) {
        if (first == n) {            /* single‑part partition */
            total += (seq_t)1;
            continue;
        }
        for (idx_t second = 1; second <= first; ++second) {
            idx_t rem = n - first - second;
            if (rem == 0) {
                total += (seq_t)1;    /* two‑part partition */
            } else {
                total += count_rem(rem, first, second);
            }
        }
    }
    return total;
}
