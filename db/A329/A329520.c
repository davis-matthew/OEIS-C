/*
 * A329520  a(n) is the number of completed steps before being trapped for a
 *          knight moving on a square-spiral numbered board where the knight
 *          moves to an unvisited square with the lowest spiral number and
 *          with n or fewer visited neighbors. It only moves to squares with
 *          n+1 or more visited neighbors when no o...
 * https://oeis.org/A329520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, walk
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Knight spiral walk length simulation
 *  Implements a(n) as the number of completed steps a knight makes on a
 *  square‑spiral‐numbered board following the rules from OEIS A329520.
 *
 *  The routine performs an on‑the‑fly simulation using a hash table
 *  mapping coordinates to visitation status and visited‑neighbour counts.
 *  For each move it selects an unvisited neighbour with <= n visited
 *  neighbours and the lowest spiral number; if none exist, it chooses
 *  from the remaining neighbours the one with fewest visited neighbours
 *  (breaking ties by lowest spiral number).
 *
 *  The simulation runs until no unvisited knight‑move squares are
 *  available, at which point the number of steps taken is returned.
 *
 *  Complexity:  O(a(n)) time, small memory per visited square.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* --- Types from the build -------------------------------------------- */
typedef uint64_t idx_t;      /* index type                     */
                /* seq_t already defined by the build   */

/* --- Utility functions ------------------------------------------------ */
static inline uint64_t hash64(uint64_t x)
{
    /* 64‑bit mix (Brent K. and Knuth) */
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

static inline uint64_t key_from_xy(int32_t x, int32_t y)
{
    return ((uint64_t)(uint32_t)x << 32) | (uint32_t)y;
}

/* --- Spiral number of (x,y) ------------------------------------------- */
static inline uint64_t spiral_number(int32_t x, int32_t y)
{
    if (x == 0 && y == 0) return 1;
    int32_t ax = x >= 0 ? x : -x;
    int32_t ay = y >= 0 ? y : -y;
    int32_t m  = ax > ay ? ax : ay;            /* ring number           */
    int64_t side   = 2LL * m + 1;              /* side length           */
    int64_t max_n  = side * side;             /* number at bottom‑right */

    if (y == -m) {              /* bottom side, moving left */
        int64_t offset = m - x;
        return max_n - offset;
    }
    if (x == -m) {              /* left side, moving up    */
        int64_t offset = 2LL * m + (y + m);
        return max_n - offset;
    }
    if (y == m) {               /* top side, moving right  */
        int64_t offset = 4LL * m + (x + m);
        return max_n - offset;
    }
    /* right side, moving down (x==m) */
    int64_t offset = 6LL * m + (m - y);
    return max_n - offset;
}

/* --- Hash table for coordinates --------------------------------------- */
typedef struct {
    size_t    capacity;     /* power of two */
    size_t    size;         /* number of used slots */
    size_t    mask;         /* capacity-1       */
    uint64_t* keys;
    uint8_t*  neigh;        /* visited neighbour count (0‑8) */
    uint8_t*  visited;      /* 0/1 */
    uint8_t*  used;         /* 0/1 */
} HashTbl;

static HashTbl* ht_init(void)
{
    HashTbl* ht = (HashTbl*)malloc(sizeof(HashTbl));
    if (!ht) return NULL;
    ht->capacity = (1U << 20);          /* start with 1M slots      */
    ht->size     = 0;
    ht->mask     = ht->capacity - 1;
    ht->keys     = (uint64_t*)calloc(ht->capacity, sizeof(uint64_t));
    ht->neigh    = (uint8_t*)calloc(ht->capacity, sizeof(uint8_t));
    ht->visited  = (uint8_t*)calloc(ht->capacity, sizeof(uint8_t));
    ht->used     = (uint8_t*)calloc(ht->capacity, sizeof(uint8_t));
    return ht;
}

static void ht_free(HashTbl* ht)
{
    if (!ht) return;
    free(ht->keys);
    free(ht->neigh);
    free(ht->visited);
    free(ht->used);
    free(ht);
}

/* rehashing: double table size */
static void ht_rehash(HashTbl* ht)
{
    size_t old_cap = ht->capacity;
    uint64_t* old_keys = ht->keys;
    uint8_t*   old_neigh = ht->neigh;
    uint8_t*   old_vis   = ht->visited;
    uint8_t*   old_used  = ht->used;

    ht->capacity <<= 1;
    ht->mask      = ht->capacity - 1;
    ht->keys      = (uint64_t*)calloc(ht->capacity, sizeof(uint64_t));
    ht->neigh     = (uint8_t*)calloc(ht->capacity, sizeof(uint8_t));
    ht->visited   = (uint8_t*)calloc(ht->capacity, sizeof(uint8_t));
    ht->used      = (uint8_t*)calloc(ht->capacity, sizeof(uint8_t));
    ht->size      = 0;

    for (size_t i = 0; i < old_cap; ++i)
        if (old_used[i]) {
            uint64_t key = old_keys[i];
            size_t idx = hash64(key) & ht->mask;
            while (ht->used[idx]) idx = (idx + 1) & ht->mask;
            ht->used[idx]   = 1;
            ht->keys[idx]   = key;
            ht->neigh[idx]  = old_neigh[i];
            ht->visited[idx] = old_vis[i];
            ht->size++;
        }

    free(old_keys);
    free(old_neigh);
    free(old_vis);
    free(old_used);
}

/* probe index (no insertion) */
static size_t ht_probe(HashTbl* ht, uint64_t key)
{
    size_t idx = hash64(key) & ht->mask;
    while (ht->used[idx] && ht->keys[idx] != key)
        idx = (idx + 1) & ht->mask;
    return idx;
}

/* find slot, inserting if necessary */
static size_t ht_find_or_insert(HashTbl* ht, uint64_t key)
{
    /* ensure enough space */
    if (ht->size + 1 > (size_t)(ht->capacity * 6 / 10)) ht_rehash(ht);

    size_t idx = hash64(key) & ht->mask;
    while (ht->used[idx]) {
        if (ht->keys[idx] == key) return idx;
        idx = (idx + 1) & ht->mask;
    }
    /* empty slot -> insert */
    ht->used[idx]   = 1;
    ht->keys[idx]   = key;
    ht->neigh[idx]  = 0;
    ht->visited[idx] = 0;
    ht->size++;
    return idx;
}

/* increment neighbour count for a coordinate (creates entry if missing) */
static void ht_inc_neigh(HashTbl* ht, int32_t x, int32_t y)
{
    uint64_t key = key_from_xy(x, y);
    size_t idx = ht_find_or_insert(ht, key);
    ht->neigh[idx]++;          /* neighbour counts never exceed 8 */
}

/* mark coordinate as visited (keeps existing neighbour count) */
static void ht_mark_visited(HashTbl* ht, int32_t x, int32_t y)
{
    uint64_t key = key_from_xy(x, y);
    size_t idx = ht_probe(ht, key);
    if (!ht->used[idx] || ht->keys[idx] != key) {
        /* entry does not exist yet – insert with count 0 */
        idx = ht_find_or_insert(ht, key);
    }
    ht->visited[idx] = 1;
}

/* retrieve neighbour count and visited flag (if not present, 0 and false) */
static void ht_get_info(HashTbl* ht, int32_t x, int32_t y,
                        bool* visited, uint8_t* neigh)
{
    uint64_t key = key_from_xy(x, y);
    size_t idx = ht_probe(ht, key);
    if (!ht->used[idx] || ht->keys[idx] != key) {
        *visited = false;
        *neigh   = 0;
    } else {
        *visited = ht->visited[idx];
        *neigh   = ht->neigh[idx];
    }
}

/* --- Knight simulation ------------------------------------------------ */
static const int32_t dx[8] = {  1,  2,  2,  1, -1, -2, -2, -1 };
static const int32_t dy[8] = {  2,  1, -1, -2, -2, -1,  1,  2 };

seq_t a(idx_t n)
{
    idx_t limit = n;          /* threshold for “low” neighbours */
    HashTbl* ht = ht_init();
    if (!ht) return (seq_t)0;               /* allocation failure (unlikely) */

    /* start at origin */
    int32_t curx = 0, cury = 0;
    ht_mark_visited(ht, curx, cury);
    /* Count neighbours of origin for future moves */
    for (int i = 0; i < 8; ++i)
        ht_inc_neigh(ht, curx + dx[i], cury + dy[i]);

    idx_t steps = 0;

    /* simulate until trapped */
    while (1) {
        int32_t best_low_x = 0,  best_low_y = 0;
        uint64_t best_low_spiral = (uint64_t)-1;
        bool has_low = false;

        int32_t best_high_x = 0, best_high_y = 0;
        uint64_t best_high_spiral = (uint64_t)-1;
        uint8_t best_high_neigh = 255;
        bool has_high = false;

        /* examine all 8 knights moves */
        for (int i = 0; i < 8; ++i) {
            int32_t nx = curx + dx[i];
            int32_t ny = cury + dy[i];
            bool visited;
            uint8_t neighcnt;
            ht_get_info(ht, nx, ny, &visited, &neighcnt);
            if (visited) continue;

            uint64_t sp = spiral_number(nx, ny);

            if ((idx_t)neighcnt <= limit) {
                if (!has_low || sp < best_low_spiral) {
                    has_low = true;
                    best_low_spiral = sp;
                    best_low_x = nx;
                    best_low_y = ny;
                }
            } else {
                if (!has_high ||
                    neighcnt < best_high_neigh ||
                    (neighcnt == best_high_neigh && sp < best_high_spiral)) {
                    has_high = true;
                    best_high_neigh = neighcnt;
                    best_high_spiral = sp;
                    best_high_x = nx;
                    best_high_y = ny;
                }
            }
        }

        if (!has_low && !has_high) break; /* trapped */

        /* choose next move */
        if (has_low) {
            curx = best_low_x;
            cury = best_low_y;
        } else {
            curx = best_high_x;
            cury = best_high_y;
        }

        /* mark visited and update neighbours */
        ht_mark_visited(ht, curx, cury);
        for (int i = 0; i < 8; ++i)
            ht_inc_neigh(ht, curx + dx[i], cury + dy[i]);

        steps++;
    }

    ht_free(ht);
    return (seq_t)steps;
}
