/*
 * A324606  The x-coordinates of squares visited by knight moves on a spirally
 *          numbered board and moving to the lowest available unvisited square
 *          at each step.
 * https://oeis.org/A324606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full, walk
 * Formula:   a(n) = A174344(A316667(n)) – the x–coordinate of the square
 *            visited by a knight on a spirally numbered board when always
 *            moving to the lowest available unvisited square
 * Range:     i8 n <= 36, i16 n <= 2016, i32 n <= 2016, i64 n <= 2016,
 *            i128 n <= 2016, big any n
 * Helpers:   hash_coord
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/hash_coord.h"

/* --- Helper: spiral index of (x,y) ------------------------------------- */
static inline seq_t spiral_index(int32_t x, int32_t y)
{
    if (x == 0 && y == 0) return (seq_t)0;
    int64_t ax = x >= 0 ? (int64_t)x : -(int64_t)x;
    int64_t ay = y >= 0 ? (int64_t)y : -(int64_t)y;
    int64_t r  = ax > ay ? ax : ay;                     /* layer number   */
    int64_t first = (2 * r - 1) * (2 * r - 1);           /* (2r-1)^2       */
    int64_t offset = 0;
    if (x == r && y != -r) {                            /* right side      */
        offset = (int64_t)y + r - 1;
    } else if (y == r) {                                /* top side        */
        offset = 2 * r + (r - 1 - (int64_t)x);
    } else if (x == -r) {                               /* left side       */
        offset = 4 * r + (r - 1 - (int64_t)y);
    } else {                                            /* bottom side     */
        offset = 6 * r + ((int64_t)x + r - 1);
    }
    return (seq_t)(first + offset);
}

/* --- Simple open‑addressing hash table for visited (x,y) pairs ------------ */
struct entry {
    int32_t x;
    int32_t y;
    uint8_t used;
};
static struct entry *hashtable      = NULL;
static size_t ht_cap                = 0;
static size_t ht_used               = 0;
static size_t ht_mask               = 0;

static void ht_expand(void)
{
    size_t new_cap = ht_cap ? ht_cap * 2 : 16;
    struct entry *new_tbl = (struct entry*)malloc(new_cap * sizeof(*new_tbl));
    memset(new_tbl, 0, new_cap * sizeof(*new_tbl));
    size_t new_mask = new_cap - 1;
    for (size_t i = 0; i < ht_cap; ++i) {
        if (hashtable[i].used) {
            uint64_t h = hash_coord(hashtable[i].x, hashtable[i].y);
            size_t idx = h & new_mask;
            while (new_tbl[idx].used) idx = (idx + 1) & new_mask;
            new_tbl[idx] = hashtable[i];
        }
    }
    free(hashtable);
    hashtable = new_tbl;
    ht_cap = new_cap;
    ht_mask = new_mask;
}

static inline bool ht_contains(int32_t x, int32_t y)
{
    if (!hashtable) return false;
    uint64_t h = hash_coord(x, y);
    size_t idx = h & ht_mask;
    for (;;) {
        if (!hashtable[idx].used) return false;
        if (hashtable[idx].x == x && hashtable[idx].y == y) return true;
        idx = (idx + 1) & ht_mask;
    }
}

static inline void ht_insert(int32_t x, int32_t y)
{
    if (!hashtable) {
        ht_cap = 16;
        ht_mask = ht_cap - 1;
        hashtable = (struct entry*)calloc(ht_cap, sizeof(*hashtable));
    }
    if (ht_used * 2 >= ht_cap) ht_expand();
    uint64_t h = hash_coord(x, y);
    size_t idx = h & ht_mask;
    for (;;) {
        if (!hashtable[idx].used) {
            hashtable[idx].x = x;
            hashtable[idx].y = y;
            hashtable[idx].used = 1;
            ++ht_used;
            return;
        }
        if (hashtable[idx].x == x && hashtable[idx].y == y) return;
        idx = (idx + 1) & ht_mask;
    }
}

/* --- Sequence data ------------------------------------------------------ */
static seq_t *seq  = NULL;      /* stored x‑coordinates */
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static int32_t cur_x  = 0, cur_y = 0;
static bool initialized = false;

static void seq_grow(size_t min_cap)
{
    if (seq_cap >= min_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    seq = (seq_t*)realloc(seq, new_cap * sizeof(*seq));
    seq_cap = new_cap;
}

/* Knight move deltas                                    */
static const int32_t knight_dx[8] = { 2,  1, -1, -2, -2, -1,  1,  2};
static const int32_t knight_dy[8] = { 1,  2,  2,  1, -1, -2, -2, -1};

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* safety, though n≥1 by definition */

    if (!initialized) {
        /* start walk: visit (0,0) */
        seq_grow(1);
        seq[0] = (seq_t)0;
        seq_len = 1;
        cur_x = 0; cur_y = 0;
        ht_insert(cur_x, cur_y);
        initialized = true;
    }

    /* compute up to desired n */
    while (seq_len < n) {
        int32_t best_x = 0, best_y = 0;
        seq_t best_idx = 0;
        bool best_found = false;

        for (int i = 0; i < 8; ++i) {
            int32_t nx = cur_x + knight_dx[i];
            int32_t ny = cur_y + knight_dy[i];
            if (ht_contains(nx, ny)) continue;
            seq_t idx = spiral_index(nx, ny);
            if (!best_found || idx < best_idx) {
                best_found = true;
                best_idx = idx;
                best_x = nx;
                best_y = ny;
            }
        }
        /* advance */
        ht_insert(best_x, best_y);
        seq_grow(seq_len + 1);
        seq[seq_len] = (seq_t)best_x;    /* store x coordinate          */
        ++seq_len;
        cur_x = best_x;
        cur_y = best_y;
    }
    return seq[n - 1];
}
