/*
 * A326922  Squares visited by a knight moving on a board with squares labeled
 *          with their squared distance from the origin and where the knight
 *          moves to the smallest labeled unvisited square; the smallest
 *          spiral number ordering is used if the distances are equal.
 * https://oeis.org/A326922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the sequence of squared distances from the origin
 *            visited by a knight that always moves to the smallest unvisited
 *            square (according to squared Euclidean distance, tie‑broken by
 *            the spiral index). The function generates the sequence by
 *            simulation
 * Range:     u8 n <= 629, u16 n <= 9710, u32 n <= 9710, u64 n <= 9710,
 *            u128 n <= 9710, big any n
 * Helpers:   hash_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/hash_pair.h"

#ifndef max64
#define max64(a,b) ((a) > (b) ? (a) : (b))
#endif

/* ------- hash table for visited squares ---------------------------------- */

typedef struct {
    int64_t *x, *y;          /* coordinates */
    unsigned char *used;     /* 0 = empty, 1 = occupied */
    size_t cap;
    size_t cnt;
} Visited;

static void visited_init(Visited *v, size_t cap)
{
    v->cap = cap;
    v->cnt = 0;
    v->x   = (int64_t *)calloc(cap, sizeof(int64_t));
    v->y   = (int64_t *)calloc(cap, sizeof(int64_t));
    v->used= (unsigned char *)calloc(cap, sizeof(unsigned char));
}

static void visited_rehash(Visited *v)
{
    size_t new_cap = v->cap << 1;
    int64_t *new_x = (int64_t *)calloc(new_cap, sizeof(int64_t));
    int64_t *new_y = (int64_t *)calloc(new_cap, sizeof(int64_t));
    unsigned char *new_used = (unsigned char *)calloc(new_cap, sizeof(unsigned char));

    for (size_t i = 0; i < v->cap; ++i) {
        if (!v->used[i]) continue;
        int64_t xx = v->x[i];
        int64_t yy = v->y[i];
        uint64_t h = hash_pair(xx, yy);
        size_t idx = h & (new_cap - 1);
        while (new_used[idx]) idx = (idx + 1) & (new_cap - 1);
        new_used[idx] = 1;
        new_x[idx] = xx;
        new_y[idx] = yy;
    }
    free(v->x); free(v->y); free(v->used);
    v->x = new_x; v->y = new_y; v->used = new_used;
    v->cap = new_cap;
}

static inline bool visited_contains(const Visited *v, int64_t x, int64_t y)
{
    uint64_t h = hash_pair(x, y);
    size_t idx = h & (v->cap - 1);
    while (v->used[idx]) {
        if (v->x[idx] == x && v->y[idx] == y) return true;
        idx = (idx + 1) & (v->cap - 1);
    }
    return false;
}

static void visited_add(Visited *v, int64_t x, int64_t y)
{
    if (v->cnt * 3 >= v->cap * 2) visited_rehash(v);
    uint64_t h = hash_pair(x, y);
    size_t idx = h & (v->cap - 1);
    while (v->used[idx]) {
        if (v->x[idx] == x && v->y[idx] == y) return;
        idx = (idx + 1) & (v->cap - 1);
    }
    v->used[idx] = 1;
    v->x[idx] = x;
    v->y[idx] = y;
    v->cnt++;
}

/* ------- spiral index (Ulam spiral) ------------------------------------- */

static seq_t spiral_index(int64_t x, int64_t y)
{
    if (x == 0 && y == 0) return (seq_t)0;

    int64_t r = max64((int64_t)(x<0?-x:x), (int64_t)(y<0?-y:y));
    seq_t rr = (seq_t)r;
    seq_t two_r_minus1 = (seq_t)2 * rr - (seq_t)1;
    seq_t start = two_r_minus1 * two_r_minus1;

    if (x == r && y != -r) {                     /* right side (upwards) */
        seq_t offset = (seq_t)(y + r - 1);
        return start + offset;
    }
    if (y == r) {                                 /* top side (leftwards) */
        seq_t offset = (seq_t)(r - 1 - x);
        return start + (seq_t)2 * rr + offset;
    }
    if (x == -r) {                                /* left side (downwards) */
        seq_t offset = (seq_t)(r - 1 - y);
        return start + (seq_t)4 * rr + offset;
    }
    /* bottom side (rightwards) */
    seq_t offset = (seq_t)(x + r - 1);
    return start + (seq_t)6 * rr + offset;
}

/* ------- cache for previously computed terms ---------------------------- */

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

static void cache_init(size_t init_cap)
{
    cache_cap = init_cap;
    cache_size = 1;
    seq_cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    seq_cache[0] = (seq_t)0;      /* a(0) = 0 */
}

static void cache_append(seq_t val)
{
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        cache_cap = new_cap;
    }
    seq_cache[cache_size++] = val;
}

/* ------- knight moves --------------------------------------------------- */

static const int8_t MOVE_X[8] = {  1,  2,  2,  1, -1, -2, -2, -1 };
static const int8_t MOVE_Y[8] = {  2,  1, -1, -2, -2, -1,  1,  2 };

/* ------- sequence function ---------------------------------------------- */

seq_t a(idx_t n)
{
    static bool initialized = false;
    static Visited visited;
    static int64_t cur_x = 0, cur_y = 0;

    if (!initialized) {
        visited_init(&visited, 1 << 15);
        visited_add(&visited, 0, 0);
        cache_init(128);
        initialized = true;
    }

    /* extend cache up to the requested index */
    while ((size_t)n >= cache_size) {
        int64_t best_x = 0, best_y = 0;
        seq_t best_d = 0;
        seq_t best_spiral = 0;
        bool found = false;

        for (int i = 0; i < 8; ++i) {
            int64_t nx = cur_x + (int64_t)MOVE_X[i];
            int64_t ny = cur_y + (int64_t)MOVE_Y[i];

            if (visited_contains(&visited, nx, ny)) continue;

            seq_t snx = (seq_t)nx;
            seq_t sny = (seq_t)ny;
            seq_t d = snx * snx + sny * sny;
            seq_t s = spiral_index(nx, ny);

            if (!found || d < best_d) {
                found = true;
                best_d = d;
                best_spiral = s;
                best_x = nx;
                best_y = ny;
            } else if (d == best_d && s < best_spiral) {
                best_spiral = s;
                best_x = nx;
                best_y = ny;
            }
        }

        if (!found) {                    /* sequence terminates */
            return (seq_t)0;
        }

        cache_append(best_d);
        visited_add(&visited, best_x, best_y);
        cur_x = best_x;
        cur_y = best_y;
    }

    return seq_cache[(size_t)n];
}
