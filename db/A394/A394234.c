/*
 * A394234  The number of squares visited by a knight before being trapped
 *          when moving on a diagonally numbered board (Cf. A316588) when
 *          starting from square n and always moving to the lowest available
 *          unvisited square, or -1 if the knight is never trapped.
 * https://oeis.org/A394234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 none, i16 n <= 4373, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   infinite_start
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A394234: number of squares visited by a knight on a
   diagonally numbered board before being trapped, starting from
   square n and always moving to the lowest available unvisited
   square.  Returns -1 if the path is infinite.  For any n satisfying
   n = 42 or n = 2*t*(4*t-1) (t ≥ 1) the path is infinite. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/infinite_start.h"

/* -------------------------------------------------------------------------- */
/* Helper: triangular number T_s = s*(s+1)/2 - computed in 128‑bit to avoid
   overflow, then cast back to 64‑bit. */
static inline uint64_t tri(uint64_t s)
{
    return (uint64_t)(((__int128)s * (s + 1)) / 2);
}

/* -------------------------------------------------------------------------- */
/* Convert board square number n (≥1) to (x, y) coordinates on the
   diagonally numbered board. */
static inline void n_to_xy(uint64_t n, uint64_t *x, uint64_t *y)
{
    uint64_t sum = 0;
    uint64_t s   = 0;
    while (true) {
        uint64_t next = sum + (s + 1);          /* sum of first s+1 numbers */
        if (n <= next) break;
        sum = next;
        ++s;
    }
    uint64_t offset = n - sum - 1;
    *x = offset;
    *y = s - offset;
}

/* -------------------------------------------------------------------------- */
/* Convert (x, y) coordinates back to board square number. */
static inline uint64_t xy_to_n(uint64_t x, uint64_t y)
{
    uint64_t s     = x + y;
    uint64_t tri_s = tri(s);
    return tri_s + x + 1;
}

/* -------------------------------------------------------------------------- */
/* Simple open‑addressing hash set for uint64_t keys (0 is unused). */
typedef struct {
    uint64_t *keys;  /* array of stored keys; 0 means empty slot */
    size_t   cap;    /* capacity of array (power of two) */
    size_t   cnt;    /* number of elements stored */
} HashSet;

static inline void hs_init(HashSet *hs)
{
    hs->cap = 16;
    hs->cnt = 0;
    hs->keys = (uint64_t *)calloc(hs->cap, sizeof(uint64_t));
}

static inline void hs_free(HashSet *hs)
{
    free(hs->keys);
}

static inline void hs_resize(HashSet *hs)
{
    size_t newcap = hs->cap << 1;
    uint64_t *newkeys = (uint64_t *)calloc(newcap, sizeof(uint64_t));
    for (size_t i = 0; i < hs->cap; ++i) {
        uint64_t k = hs->keys[i];
        if (k) {
            size_t h = k & (newcap - 1);
            while (newkeys[h]) h = (h + 1) & (newcap - 1);
            newkeys[h] = k;
        }
    }
    free(hs->keys);
    hs->keys = newkeys;
    hs->cap  = newcap;
}

static inline void hs_insert(HashSet *hs, uint64_t key)
{
    if (hs->cnt * 2 >= hs->cap) hs_resize(hs);
    size_t h = key & (hs->cap - 1);
    while (hs->keys[h]) h = (h + 1) & (hs->cap - 1);
    hs->keys[h] = key;
    ++hs->cnt;
}

static inline bool hs_contains(const HashSet *hs, uint64_t key)
{
    size_t h = key & (hs->cap - 1);
    while (hs->keys[h]) {
        if (hs->keys[h] == key) return true;
        h = (h + 1) & (hs->cap - 1);
    }
    return false;
}

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t start = (uint64_t)n;

    if (infinite_start(start)) return (seq_t)-1;

    HashSet visited;
    hs_init(&visited);
    hs_insert(&visited, start);
    size_t visited_cnt = 1;

    /* Knight moves (dx, dy) */
    static const int8_t dx[8] = {  1,  2,  2,  1, -1, -2, -2, -1 };
    static const int8_t dy[8] = {  2,  1, -1, -2, -2, -1,  1,  2 };

    uint64_t cur = start;

    while (true) {
        uint64_t cx, cy;
        n_to_xy(cur, &cx, &cy);

        uint64_t best = (uint64_t)(-1);

        for (int i = 0; i < 8; ++i) {
            int64_t nx = (int64_t)cx + dx[i];
            int64_t ny = (int64_t)cy + dy[i];
            if (nx < 0 || ny < 0) continue;

            uint64_t next = xy_to_n((uint64_t)nx, (uint64_t)ny);
            if (hs_contains(&visited, next)) continue;

            if (next < best) best = next;
        }

        if (best == (uint64_t)(-1)) {
            hs_free(&visited);
            return (seq_t)visited_cnt;
        }

        cur = best;
        hs_insert(&visited, cur);
        ++visited_cnt;
    }
}
