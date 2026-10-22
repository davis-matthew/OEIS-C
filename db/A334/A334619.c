/*
 * A334619  Squares visited on a board numbered in counterclockwise square
 *          spiral by successively turning to the left square if it is
 *          unvisited or to the right square otherwise.
 * https://oeis.org/A334619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is produced by a left‑turning walk on an integer * grid
 *            whose squares are numbered by the counter‑clockwise spiral *
 *            mapping. Starting at the origin, the walk starts looking south;
 *            * at each step it turns left if the left square is unvisited, *
 *            otherwise it turns right. The returned value is...
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   absl
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/absl.h"

/* spiral number of coordinate (x,y) */
static inline seq_t spiral_number(int64_t x, int64_t y) {
    if (x == 0 && y == 0) return (seq_t)1;
    idx_t ax = (idx_t)absl(x), ay = (idx_t)absl(y);
    idx_t r = (ax > ay) ? ax : ay;
    seq_t sideLen = (seq_t)(2 * r);
    seq_t start = (seq_t)(2 * r - 1);
    start = start * start + (seq_t)1;
    seq_t offset;
    if (x == (int64_t)r && y != -(int64_t)r) {
        offset = (seq_t)(y + (int64_t)r - 1);
    } else if (y == (int64_t)r && x != (int64_t)r) {
        offset = sideLen + (seq_t)(r - (idx_t)x - 1);
    } else if (x == -(int64_t)r && y != (int64_t)r) {
        offset = 2 * sideLen + (seq_t)(r - (idx_t)y - 1);
    } else {   /* y == -(int64_t)r */
        offset = 3 * sideLen + (seq_t)(x + (int64_t)r - 1);
    }
    return start + offset;
}

/* tiny hash table for visited coordinates */
struct VisEntry {
    int64_t x, y;
    int used;
};

static inline size_t hash_pair(int64_t x, int64_t y, size_t mask) {
    /* 32‑bit multiplicative hash */
    uint64_t h = ((uint64_t)(int32_t)x * 0x9e3779b97f4a7c15ULL) ^
                 ((uint64_t)(int32_t)y * 0xbf58476d1ce4e5b9ULL);
    return (size_t)h & mask;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;       /* undefined for 0 */
    if (n == 1) return (seq_t)1;

    /* size of hash table: power of two >= 2*n + 3 */
    size_t cap = 2;
    while (cap < 2 * (size_t)n + 3) cap <<= 1;
    size_t mask = cap - 1;

    struct VisEntry *table = (struct VisEntry *)calloc(cap, sizeof(struct VisEntry));
    if (!table) return (seq_t)0; /* allocation failed, return 0 as placeholder */

    /* initial position and direction (south) */
    int64_t x = 0, y = 0;
    int64_t dx = 0, dy = -1;

    /* insert origin */
    size_t h = hash_pair(x, y, mask);
    while (table[h].used) h = (h + 1) & mask;
    table[h].x = x; table[h].y = y; table[h].used = 1;

    seq_t result = (seq_t)1;  /* current a(1) */

    for (idx_t step = 1; step < n; ++step) {
        /* compute left direction: (-dy, dx) */
        int64_t ldx = -dy, ldy = dx;
        /* compute right direction: (dy, -dx) */
        int64_t rdx = dy, rdy = -dx;

        int64_t lx = x + ldx, ly = y + ldy;
        int64_t rx = x + rdx, ry = y + rdy;

        int left_visited = 0, right_visited = 0;
        /* check left */
        h = hash_pair(lx, ly, mask);
        while (table[h].used) {
            if (table[h].x == lx && table[h].y == ly) { left_visited = 1; break; }
            h = (h + 1) & mask;
        }
        /* check right */
        h = hash_pair(rx, ry, mask);
        while (table[h].used) {
            if (table[h].x == rx && table[h].y == ry) { right_visited = 1; break; }
            h = (h + 1) & mask;
        }

        if (!left_visited) {
            x = lx; y = ly;
            dx = ldx; dy = ldy;
        } else {
            x = rx; y = ry;
            dx = rdx; dy = rdy;
        }

        /* insert new position */
        h = hash_pair(x, y, mask);
        while (table[h].used) h = (h + 1) & mask;
        table[h].x = x; table[h].y = y; table[h].used = 1;

        result = spiral_number(x, y);
    }

    free(table);
    return result;
}
