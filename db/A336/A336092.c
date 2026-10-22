/*
 * A336092  Squares visited by a chess king moving on a square-spiral numbered
 *          board where the king moves to the adjacent unvisited square
 *          containing the spiral number with the fewest divisors. In case of
 *          a tie it chooses the square with the largest spiral number.
 * https://oeis.org/A336092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   divisor_count_u64, next_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A336092:
 *   a(1) = 1.
 *   A king starts on square 1 of a square‑spiral board.  At each step it
 *   moves to an adjacent unvisited square that has the fewest divisors;
 *   if several squares tie, the king chooses the one with the largest
 *   spiral number.  The function returns the spiral number visited at
 *   step n.  The sequence is finite; the algorithm stops when no
 *   unvisited neighbour exists. */

/*  Dependencies from the utilities set   */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/next_pow2.h"

/* Simple open‑addressing hash set for 64‑bit keys   */
typedef struct {
    uint64_t *keys;   /* 0 denotes empty slot */
    size_t   capacity;
    size_t   size;
} HashSet;

static void hs_init(HashSet *hs, size_t capacity) {
    capacity = next_pow2(capacity);
    hs->keys = (uint64_t *)calloc(capacity, sizeof(uint64_t));
    hs->capacity = capacity;
    hs->size = 0;
}

static void hs_resize(HashSet *hs) {
    size_t new_cap = hs->capacity << 1;
    uint64_t *new_keys = (uint64_t *)calloc(new_cap, sizeof(uint64_t));
    for (size_t i = 0; i < hs->capacity; ++i) {
        uint64_t k = hs->keys[i];
        if (!k) continue;
        size_t idx = k & (new_cap - 1);
        while (new_keys[idx]) idx = (idx + 1) & (new_cap - 1);
        new_keys[idx] = k;
    }
    free(hs->keys);
    hs->keys = new_keys;
    hs->capacity = new_cap;
}

static bool hs_find(const HashSet *hs, uint64_t key) {
    size_t idx = key & (hs->capacity - 1);
    while (hs->keys[idx]) {
        if (hs->keys[idx] == key) return true;
        idx = (idx + 1) & (hs->capacity - 1);
    }
    return false;
}

static void hs_insert(HashSet *hs, uint64_t key) {
    if ((hs->size + 1ULL) * 5ULL / 4ULL > hs->capacity) hs_resize(hs);
    size_t idx = key & (hs->capacity - 1);
    while (hs->keys[idx]) idx = (idx + 1) & (hs->capacity - 1);
    hs->keys[idx] = key;
    ++hs->size;
}

/* Convert cartesian coordinates (x,y) to the spiral number.  The
 * coordinates are relative to the centre of the spiral, for which
 * (0,0) maps to 1.  All arithmetic is done in unsigned 64‑bit
 * integers to prevent overflow for the board sizes needed by the
 * sequence. */
static inline uint64_t coord_to_num(long long x, long long y) {
    if (x == 0 && y == 0) return 1ULL;

    long long ax = x >= 0 ? x : -x;
    long long ay = y >= 0 ? y : -y;
    long long k  = ax > ay ? ax : ay;          /* ring index */

    unsigned long long side   = 2ULL * k + 1ULL;                 /* side length of this ring */
    unsigned long long prev   = (side - 2ULL) * (side - 2ULL);   /* last number of previous ring */
    unsigned long long offset = 0ULL;

    if (x == k && y != -k) {                      /* right side, moving up */
        offset = (unsigned long long)(y + (k - 1));
    } else if (y == k) {                          /* top side, moving left */
        offset = (unsigned long long)(2 * k + (k - 1 - x));
    } else if (x == -k) {                         /* left side, moving down */
        offset = (unsigned long long)(4 * k + (k - 1 - y));
    } else {                                      /* bottom side, moving right (y == -k) */
        offset = (unsigned long long)(6 * k + (x + k - 1));
    }
    return prev + offset + 1ULL;
}

/* Main function required by the test harness.   */
seq_t a(idx_t n)
{
    /* static state for memoised walk */
    static seq_t *seq   = NULL;
    static idx_t seq_len = 0, seq_cap = 0;
    static HashSet visited;
    static long long cur_x = 0, cur_y = 0;
    static bool initialized = false;

    if (!initialized) {
        /* initial allocation for the sequence array */
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;          /* a(1) = 1 */
        seq_len = 1;

        /* initialise visited set */
        visited.capacity = 2048;
        hs_init(&visited, visited.capacity);
        hs_insert(&visited, 1ULL);

        cur_x = 0; cur_y = 0;
        initialized = true;
    }

    /* extend the walk until we have at least n terms */
    while (seq_len < n) {
        uint64_t best_num   = 0;
        uint64_t best_divs  = UINT64_MAX;
        long long best_dx   = 0;
        long long best_dy   = 0;

        for (long long dx = -1; dx <= 1; ++dx) {
            for (long long dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                long long nx = cur_x + dx;
                long long ny = cur_y + dy;
                uint64_t num = coord_to_num(nx, ny);
                if (hs_find(&visited, num)) continue;
                uint64_t d = divisor_count_u64(num);
                if (d < best_divs || (d == best_divs && num > best_num)) {
                    best_divs = d;
                    best_num  = num;
                    best_dx   = dx;
                    best_dy   = dy;
                }
            }
        }

        /* if there is no legal move we stop early   */
        if (best_num == 0) break;

        /* ensure enough space in the sequence array */
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len] = (seq_t)best_num;  /* casting may truncate, but is OK
                                           * for terms that fit in seq_t */
        ++seq_len;

        /* mark visited and update current position */
        hs_insert(&visited, best_num);
        cur_x += best_dx;
        cur_y += best_dy;
    }

    return seq[n - 1];
}
