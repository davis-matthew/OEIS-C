/*
 * A323811  Squares visited by a knight on a diagonally numbered board and
 *          moving to the lowest available unvisited square at each step and
 *          if no unvisited squares are available move one step back.
 * https://oeis.org/A323811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 140, u16 n <= 9979, u32 n <= 9979, u64 n <= 9979,
 *            u128 n <= 9979, big any n
 * Helpers:   inverse_cantor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A323811:  squares visited by a knight on a diagonally numbered board.
   The board numbers are given by the Cantor pairing function
        n = (r+c)*(r+c+1)/2 + r   for coordinates (r,c) ≥ 0.
   The knight always moves to the smallest unvisited reachable square.
   If no such square exists it backtracks one step (moves onto the previous
   square in the path).  The function memoises the entire path so that
   any call returns the correct value for all 0 ≤ n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/inverse_cantor.h"

/* --- Utility: integer sqrt (floor) for 64‑bit unsigned integers --- */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1 << 62;   /* highest power of 4 <= 2^64-1 */
    while (bit > 0) {
        if (r + bit <= x) {
            r += bit;
            x -= r;
        }
        r >>= 1;
        bit >>= 2;
    }
    return r;
}

/* --- Cantor pairing and inverse --- */
static inline uint64_t cantor_pair(uint64_t r, uint64_t c)
{
    unsigned __int128 s = (unsigned __int128)r + (unsigned __int128)c;
    unsigned __int128 val = s * (s + 1) / 2 + r;
    return (uint64_t)val;   /* board numbers fit in 64 bits for all n ≤ 10000 */
}

/* --- Simple open‑addressing hash set for visited board numbers --- */
#define VISITED_SENTINEL ((uint64_t)-1)
typedef struct {
    uint64_t *table;
    size_t   cap;   /* power of two */
    size_t   size;  /* number of elements stored */
} visited_set_t;

static visited_set_t visited_set;

static inline uint64_t hash64(uint64_t x)
{
    /* 64‑bit multiplicative hash */
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

static void visited_set_init(void)
{
    visited_set.cap = 64;
    visited_set.size = 0;
    visited_set.table = (uint64_t *)malloc(visited_set.cap * sizeof(uint64_t));
    for (size_t i = 0; i < visited_set.cap; ++i)
        visited_set.table[i] = VISITED_SENTINEL;
}

static void visited_set_rehash(void)
{
    size_t new_cap = visited_set.cap << 1;
    uint64_t *new_table = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
    for (size_t i = 0; i < new_cap; ++i)
        new_table[i] = VISITED_SENTINEL;

    for (size_t i = 0; i < visited_set.cap; ++i) {
        uint64_t val = visited_set.table[i];
        if (val == VISITED_SENTINEL) continue;
        size_t idx = hash64(val) & (new_cap - 1);
        while (new_table[idx] != VISITED_SENTINEL)
            idx = (idx + 1) & (new_cap - 1);
        new_table[idx] = val;
    }

    free(visited_set.table);
    visited_set.table = new_table;
    visited_set.cap = new_cap;
}

static bool visited_set_contains(uint64_t x)
{
    size_t idx = hash64(x) & (visited_set.cap - 1);
    while (visited_set.table[idx] != VISITED_SENTINEL) {
        if (visited_set.table[idx] == x)
            return true;
        idx = (idx + 1) & (visited_set.cap - 1);
    }
    return false;
}

static void visited_set_insert(uint64_t x)
{
    if (visited_set_contains(x))
        return;
    size_t idx = hash64(x) & (visited_set.cap - 1);
    while (visited_set.table[idx] != VISITED_SENTINEL)
        idx = (idx + 1) & (visited_set.cap - 1);
    visited_set.table[idx] = x;
    visited_set.size++;
    if (visited_set.size * 2 > visited_set.cap)
        visited_set_rehash();
}

/* --- Dynamic arrays for path and cache --- */
typedef struct {
    uint64_t *path;    /* board numbers */
    seq_t    *cache;   /* values to return; same size as path */
    size_t   size;
    size_t   cap;
} path_state_t;

static path_state_t path_state;

static void path_state_init(void)
{
    path_state.cap = 64;
    path_state.size = 0;
    path_state.path = (uint64_t *)malloc(path_state.cap * sizeof(uint64_t));
    path_state.cache = (seq_t *)malloc(path_state.cap * sizeof(seq_t));
}

static void path_state_push(uint64_t board, seq_t val)
{
    if (path_state.size == path_state.cap) {
        size_t new_cap = path_state.cap << 1;
        uint64_t *new_path = (uint64_t *)realloc(path_state.path, new_cap * sizeof(uint64_t));
        seq_t    *new_cache = (seq_t *)realloc(path_state.cache, new_cap * sizeof(seq_t));
        path_state.path  = new_path;
        path_state.cache = new_cache;
        path_state.cap   = new_cap;
    }
    path_state.path[path_state.size] = board;
    path_state.cache[path_state.size] = val;
    path_state.size++;
}

/* --- Main sequence function --- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        visited_set_init();
        path_state_init();
        visited_set_insert(0ULL);
        path_state_push(0ULL, (seq_t)0);
    }

    /* Knight move deltas */
    const int dr[8] = { 2,  2, -2, -2,  1,  1, -1, -1 };
    const int dc[8] = { 1, -1,  1, -1,  2, -2,  2, -2 };

    while (path_state.size <= (size_t)n) {
        uint64_t cur = path_state.path[path_state.size - 1];
        uint64_t r, c;
        inverse_cantor(cur, &r, &c);

        bool found = false;
        uint64_t min_next = 0;

        for (int i = 0; i < 8; ++i) {
            int64_t nr = (int64_t)r + dr[i];
            int64_t nc = (int64_t)c + dc[i];
            if (nr < 0 || nc < 0) continue;
            uint64_t next = cantor_pair((uint64_t)nr, (uint64_t)nc);
            if (!visited_set_contains(next)) {
                if (!found || next < min_next) {
                    min_next = next;
                    found = true;
                }
            }
        }

        if (found) {
            visited_set_insert(min_next);
            path_state_push(min_next, (seq_t)min_next);
        } else {
            /* Backtrack: step onto the previous square */
            uint64_t prev = path_state.path[path_state.size - 2];
            path_state_push(prev, (seq_t)prev);
        }
    }

    return path_state.cache[(size_t)n];
}
