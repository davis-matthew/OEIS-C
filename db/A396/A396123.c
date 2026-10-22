/*
 * A396123  Consider the square spiral with its cells numbered starting at 0.
 *          Five players take turns and place knights at the smallest
 *          unoccupied cell not attacked by an existing knight of a different
 *          player. Sequence lists squares occupied by the second player.
 * https://oeis.org/A396123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 7248, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   coords_to_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A396123: squares occupied by player 2 in a 5‑player knight game on a square spiral.
   The function builds the game state incrementally and memoises the second‑player’s positions.
   Time complexity is O(N) where N is the number of cells examined. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/coords_to_idx.h"

/* --- Helper: integer square root (floor). Works for 64‑bit integers. */
static inline idx_t _floor_isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = (n >> 1) + 1;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq > n) hi = mid - 1;
        else        lo = mid;
    }
    return lo;
}

/* --- Knight moves (dx,dy) */
static const int8_t knight_moves[8][2] = {
    { 1,  2}, { 2,  1}, { 2, -1}, { 1, -2},
    {-1, -2}, {-2, -1}, {-2,  1}, {-1,  2}
};

/* --- Map index -> (x,y) 
       n starts from 0, center (0,0) = 0 */
static void idx_to_coords(idx_t n, int64_t *ox, int64_t *oy) {
    if (n == 0) { *ox = 0; *oy = 0; return; }
    idx_t r = _floor_isqrt_u64(n);
    idx_t k = ((r + 1) >> 1);      /* ring number (k >= 1) */
    idx_t ring_start = ((2 * k - 1) * (int64_t)(2 * k - 1));
    idx_t d = n - ring_start;      /* offset within the ring */

    int64_t x, y;
    if (d < 2 * k) {                      /* east side: from (k, -(k-1)) up to (k, k) */
        y = -(int64_t)(k - 1) + (int64_t)d;
        x = k;
    } else if (d < 4 * k) {                /* north side: from (k-1, k) to (-k, k) */
        idx_t d1 = d - 2 * k;
        x = k - 1 - d1;
        y = k;
    } else if (d < 6 * k) {                /* west side: from (-k, k-1) to (-k, -k) */
        idx_t d2 = d - 4 * k;
        x = -k;
        y = k - 1 - d2;
    } else {                               /* south side: from (-k+1, -k) to (k, -k) */
        idx_t d3 = d - 6 * k;
        x = -k + 1 + d3;
        y = -k;
    }
    *ox = x; *oy = y;
}

/* --- Dynamic arrays for board state */
static uint8_t *board_occ = NULL;          /* bitmask of occupying player (only one bit set) */
static uint8_t *board_att = NULL;          /* bitmask of players attacking the cell */
static size_t board_cap   = 0;             /* allocated size in cells */

static void ensure_board_cap(idx_t i) {
    if (i < board_cap) return;
    size_t newcap = board_cap ? board_cap * 2 : 16;
    while (newcap <= (size_t)i) newcap <<= 1;
    board_occ  = (uint8_t *)realloc(board_occ, newcap);
    board_att  = (uint8_t *)realloc(board_att, newcap);
    memset(board_occ + board_cap, 0, (newcap - board_cap));
    memset(board_att + board_cap, 0, (newcap - board_cap));
    board_cap = newcap;
}

/* --- Cache for second player positions */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;            /* number of entries stored */

static void cache_push(seq_t val) {
    if (cache_len >= cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }
    cache[cache_len++] = val;
}

/* --- Search cursors for each player (next index to try) */
static idx_t player_search[5] = {0, 0, 0, 0, 0};

/* --- Turn counter */
static idx_t turn = 0;           /* number of turns processed */

/* --- Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* behaviour below offset irrelevant */

    /* First call: initialise everything */
    if (!cache) {
        /* allocate initial board arrays (needs C11 realloc handle) */
        ensure_board_cap(0);
    }

    /* Extend simulation until we have at least n second‑player moves */
    while ((size_t)n > cache_len) {
        int p = turn % 5;          /* player index 0..4 (player 2 is p==1) */
        unsigned int player_bit = 1u << p;

        idx_t i, start = player_search[p];
        for (i = start; ; ++i) {
            ensure_board_cap(i);
            if (board_occ[i] == 0 && (board_att[i] & ~player_bit) == 0) {
                /* place the knight */
                board_occ[i] = player_bit;

                /* compute coordinates of the new knight */
                int64_t x, y;
                idx_to_coords(i, &x, &y);

                /* mark all attacked squares */
                for (int m = 0; m < 8; ++m) {
                    int64_t nx = x + knight_moves[m][0];
                    int64_t ny = y + knight_moves[m][1];
                    idx_t j = coords_to_idx(nx, ny);
                    ensure_board_cap(j);
                    board_att[j] |= player_bit;
                }

                /* advance this player's search cursor */
                player_search[p] = i + 1;

                /* record the square if it belongs to player 2 (p==1) */
                if (p == 1) {
                    cache_push((seq_t)i);
                }
                break;
            }
        }
        ++turn;
    }

    return cache[n-1];
}
