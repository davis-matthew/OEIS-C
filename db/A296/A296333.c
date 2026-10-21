/*
 * A296333  Number of nX6 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A296333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A296333: number of n×6 binary arrays in which every 1 has
   0 or 2 neighboring 1s among horizontal, vertical, or anti‑diagonal directions.
   The function uses dynamic programming and memoises results for any call order. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define WIDTH     6U
#define NUM_PAT   (1U << WIDTH)          /* 64 */

static inline bool row_is_valid(unsigned mid, unsigned up, unsigned down)
{
    for (unsigned col = 0; col < WIDTH; ++col) {
        if (!(mid & (1U << col))) continue;          /* only check cells with 1 */

        unsigned neigh = 0;

        /* vertical neighbors */
        if (up   & (1U << col))   ++neigh;
        if (down & (1U << col))   ++neigh;

        /* horizontal neighbors */
        if (col > 0U && (mid & (1U << (col - 1U)))) ++neigh;
        if (col < WIDTH-1U && (mid & (1U << (col + 1U)))) ++neigh;

        /* anti‑diagonal neighbors (up‑right and down‑left) */
        if (col < WIDTH-1U && (up   & (1U << (col + 1U)))) ++neigh;
        if (col > 0U   && (down & (1U << (col - 1U)))) ++neigh;

        /* each 1 must have 0 or 2 neighboring 1s */
        if (neigh != 0U && neigh != 2U) return false;
    }
    return true;
}

/* Transition tables: for each (up,mid) pair list admissible down rows. */
static bool trans_done = false;
static unsigned char trans_next[NUM_PAT * NUM_PAT][NUM_PAT];
static unsigned char trans_cnt [NUM_PAT * NUM_PAT];

static void build_transitions(void)
{
    if (trans_done) return;

    for (unsigned up = 0; up < NUM_PAT; ++up) {
        for (unsigned mid = 0; mid < NUM_PAT; ++mid) {
            unsigned idx = up * NUM_PAT + mid;
            unsigned char cnt = 0;
            for (unsigned down = 0; down < NUM_PAT; ++down) {
                if (row_is_valid(mid, up, down))
                    trans_next[idx][cnt++] = (unsigned char)down;
            }
            trans_cnt[idx] = cnt;
        }
    }
    trans_done = true;
}

/* DP state arrays */
static seq_t  dp[2][NUM_PAT * NUM_PAT];
static unsigned cur_plane = 0;
static idx_t  computed_rows = 0;     /* number of rows processed so far */

/* Memoised sequence values up to the largest n seen so far. */
static seq_t *seq_cache = NULL;      /* 1‑based, seq_cache[0] unused */
static idx_t  cache_cap   = 0;
static idx_t  cache_done  = 0;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* out of range, not used */

    build_transitions();

    /* Initialise on first call */
    if (computed_rows == 0) {
        /* Allocate initial cache */
        if (!seq_cache) {
            cache_cap = 64;
            seq_cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        }

        /* DP for first row */
        size_t total = NUM_PAT * NUM_PAT;
        for (size_t i = 0; i < total; ++i) dp[0][i] = 0;
        for (unsigned p = 0; p < NUM_PAT; ++p)
            dp[0][p] = 1;           /* state (0,p) */

        computed_rows = 1;
        cur_plane    = 0;

        /* a(1) */
        seq_t res = 0;
        for (unsigned mid = 0; mid < NUM_PAT; ++mid)
            if (row_is_valid(mid, 0, 0))
                res = res + dp[0][mid];
        seq_cache[1] = res;
        cache_done = 1;
    }

    /* Extend DP to the required number of rows */
    while (computed_rows < n) {
        unsigned next_plane = cur_plane ^ 1;
        size_t total = NUM_PAT * NUM_PAT;
        for (size_t i = 0; i < total; ++i) dp[next_plane][i] = 0;

        /* Transition */
        for (unsigned up = 0; up < NUM_PAT; ++up) {
            for (unsigned mid = 0; mid < NUM_PAT; ++mid) {
                size_t cur_idx = up * NUM_PAT + mid;
                seq_t val = dp[cur_plane][cur_idx];
                if (!val) continue;
                unsigned char *list = trans_next[cur_idx];
                unsigned char cnt   = trans_cnt[cur_idx];
                for (unsigned char t = 0; t < cnt; ++t) {
                    unsigned down = list[t];
                    size_t next_idx = mid * NUM_PAT + down;
                    dp[next_plane][next_idx] = dp[next_plane][next_idx] + val;
                }
            }
        }

        cur_plane = next_plane;
        ++computed_rows;

        /* Compute a(computed_rows) using last row validation against 0 */
        seq_t res = 0;
        for (unsigned up = 0; up < NUM_PAT; ++up) {
            for (unsigned mid = 0; mid < NUM_PAT; ++mid) {
                if (row_is_valid(mid, up, 0))
                    res = res + dp[cur_plane][up * NUM_PAT + mid];
            }
        }

        /* Ensure cache big enough */
        if ((idx_t)n >= cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
            while ((idx_t)n >= new_cap) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            seq_cache = tmp;
            cache_cap = new_cap;
        }
        seq_cache[computed_rows] = res;
        if (computed_rows > cache_done) cache_done = computed_rows;
    }

    return seq_cache[(size_t)n];
}
