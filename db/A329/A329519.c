/*
 * A329519  Squares visited by a knight moving on a square-spiral numbered
 *          board where the knight moves to an unvisited square with the
 *          lowest spiral number and with six or fewer visited neighbors. It
 *          only moves to squares with seven or more visited neighbors when no
 *          other square is available; if two or more...
 * https://oeis.org/A329519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, walk
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 9825, u32 n <= 9825, u64 n <= 9825,
 *            u128 n <= 9825, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by simulating the knight walk on the square‑spiral numbered
   board following the neighbor‑count and tie‑breaking rules described
   in A329519. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Hash table parameters */
#define TABLE_SIZE 262144            /* power of two, > expected visited squares */
#define MASK (TABLE_SIZE - 1)

/* Knight move offsets */
static const int dx[8] = {  2,  1, -1, -2, -2, -1,  1,  2 };
static const int dy[8] = {  1,  2,  2,  1, -1, -2, -2, -1 };

/* Hash table for visited squares */
static bool   vis_used[TABLE_SIZE];
static uint64_t vis_key[TABLE_SIZE];

/* Hash table for neighbour counts */
static bool   ncnt_used[TABLE_SIZE];
static uint64_t ncnt_key[TABLE_SIZE];
static uint8_t ncnt_val[TABLE_SIZE];

/* Current state of the walk */
static bool        initialized = false;
static int64_t     cur_x = 0, cur_y = 0;

/* Sequence storage */
#ifdef __cplusplus
static std::vector<seq_t> seq_vals;            /* C++ implementation */
#else
static seq_t *seq_vals = NULL;                 /* C implementation */
static size_t  seq_cap = 0;
static idx_t  seq_len = 0;
#endif

/* Ensure capacity of the C array */
#ifndef __cplusplus
static void seq_ensure(size_t needed)
{
    while (seq_cap < needed) {
        size_t newCap = seq_cap == 0 ? 64 : seq_cap << 1;
        seq_vals = (seq_t *)realloc(seq_vals, newCap * sizeof(seq_t));
        seq_cap = newCap;
    }
}
#endif

/* Return current sequence length */
static size_t seq_length(void)
{
#ifdef __cplusplus
    return seq_vals.size();
#else
    return seq_len;
#endif
}

/* Encode an (x,y) pair into a 64‑bit key */
static inline uint64_t enc_key(int64_t x, int64_t y)
{
    return ((uint64_t)(int64_t)x << 32) ^ (uint32_t)(int64_t)y;
}

/* Visited hash table operations */
static inline size_t idx_vis(uint64_t k)
{
    size_t i = (k ^ (k >> 33)) & MASK;
    while (vis_used[i]) {
        if (vis_key[i] == k) return i;
        i = (i + 1) & MASK;
    }
    return i;
}
static inline bool vis_contains(uint64_t k)
{
    size_t i = idx_vis(k);
    return vis_used[i] && vis_key[i] == k;
}
static inline void vis_insert(uint64_t k)
{
    size_t i = idx_vis(k);
    vis_used[i] = true;
    vis_key[i] = k;
}

/* Neighbour count hash table operations */
static inline size_t idx_ncnt(uint64_t k)
{
    size_t i = (k ^ (k >> 33)) & MASK;
    while (ncnt_used[i]) {
        if (ncnt_key[i] == k) return i;
        i = (i + 1) & MASK;
    }
    return i;
}
static inline uint8_t ncnt_get(uint64_t k)
{
    size_t i = idx_ncnt(k);
    return (ncnt_used[i] && ncnt_key[i] == k) ? ncnt_val[i] : 0;
}
static inline void ncnt_inc(uint64_t k)
{
    size_t i = idx_ncnt(k);
    if (ncnt_used[i] && ncnt_key[i] == k) {
        ncnt_val[i]++;  return;
    }
    ncnt_used[i] = true;
    ncnt_key[i] = k;
    ncnt_val[i] = 1;
}

/* Spiral number for a square (x,y) according to A316667. */
static inline seq_t spiral_num(int64_t x, int64_t y)
{
    if (x == 0 && y == 0) return (seq_t)1;
    int64_t ax = x >= 0 ? x : -x;
    int64_t ay = y >= 0 ? y : -y;
    int64_t k  = ax > ay ? ax : ay;
    int64_t prev = (2 * k - 1) * (2 * k - 1);          /* numbers before ring k */
    int64_t t    = 2 * k;                              /* side length */
    int64_t offset;

    if (x == k && y > -k) {
        offset = y + (k - 1);
    } else if (x == k && y == -k) {
        offset = 4 * t - 1;
    } else if (y == k) {
        offset = t + (k - 1 - x);
    } else if (x == -k) {
        offset = 2 * t + (k - 1 - y);
    } else { /* y == -k */
        offset = 3 * t + (x + (k - 1));
    }
    return (seq_t)(prev + 1 + offset);
}

/* Initialise static data on first call. */
static void init(void)
{
    if (initialized) return;
    initialized = true;

#ifdef __cplusplus
    seq_vals.clear();
    seq_vals.push_back((seq_t)1);
#else
    seq_cap = 0; seq_len = 0; seq_vals = NULL;
    seq_ensure(1);
    seq_vals[0] = (seq_t)1;
    seq_len = 1;
#endif

    cur_x = 0; cur_y = 0;
    uint64_t key0 = enc_key(0,0);
    vis_insert(key0);

    /* Increment neighbour counts for the 8 neighbours of the origin. */
    for (int i = 0; i < 8; ++i) {
        int64_t nx = cur_x + dx[i];
        int64_t ny = cur_y + dy[i];
        ncnt_inc(enc_key(nx, ny));
    }
}

/* One step of the knight walk. */
static void walk_one_step(void)
{
    int64_t cand_x[8];
    int64_t cand_y[8];
    uint8_t cand_nb[8];
    seq_t   cand_spiral[8];
    size_t  cand_cnt = 0;

    for (int i = 0; i < 8; ++i) {
        int64_t nx = cur_x + dx[i];
        int64_t ny = cur_y + dy[i];
        uint64_t k = enc_key(nx, ny);
        if (vis_contains(k)) continue;
        cand_x[cand_cnt]   = nx;
        cand_y[cand_cnt]   = ny;
        cand_nb[cand_cnt]  = ncnt_get(k);
        cand_spiral[cand_cnt] = spiral_num(nx, ny);
        ++cand_cnt;
    }

    if (cand_cnt == 0) return;   /* should not happen for n ≤ 3 935 788 */

    /* Selection according to the rules. */
    size_t best = cand_cnt;  /* sentinel */
    seq_t best_spiral = 0;
    uint8_t best_nb = 0;

    /* First look for candidates with ≤ 6 visited neighbours. */
    for (size_t i = 0; i < cand_cnt; ++i) {
        if (cand_nb[i] <= 6) {
            if (best == cand_cnt || cand_spiral[i] < best_spiral) {
                best = i;
                best_spiral = cand_spiral[i];
            }
        }
    }

    /* If none found, pick the candidate with the least neighbours;
       tie‑break by lowest spiral number. */
    if (best == cand_cnt) {
        for (size_t i = 0; i < cand_cnt; ++i) {
            if (best == cand_cnt || cand_nb[i] < best_nb ||
                (cand_nb[i] == best_nb && cand_spiral[i] < best_spiral)) {
                best = i;
                best_nb = cand_nb[i];
                best_spiral = cand_spiral[i];
            }
        }
    }

    /* Move to the chosen square */
    cur_x = cand_x[best];
    cur_y = cand_y[best];
    uint64_t chosen_key = enc_key(cur_x, cur_y);
    vis_insert(chosen_key);

#ifdef __cplusplus
    seq_vals.push_back(cand_spiral[best]);
#else
    seq_ensure(seq_len + 1);
    seq_vals[seq_len] = cand_spiral[best];
    ++seq_len;
#endif

    /* Update neighbour counts for the new square's neighbours. */
    for (int i = 0; i < 8; ++i) {
        int64_t nx = cur_x + dx[i];
        int64_t ny = cur_y + dy[i];
        ncnt_inc(enc_key(nx, ny));
    }
}

/* Public function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n < 1 */
    init();

    size_t len = seq_length();
    while (len < n) {
        walk_one_step();
        len = seq_length();
    }

#ifdef __cplusplus
    return seq_vals[n - 1];
#else
    return seq_vals[n - 1];
#endif
}
