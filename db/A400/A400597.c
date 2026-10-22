/*
 * A400597  Squares visited by a chess king moving on the square spiral,
 *          always moving to an adjacent unvisited square whose number has the
 *          greatest number of distinct prime factors; ties are resolved by
 *          choosing the lowest number.
 * https://oeis.org/A400597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, fini, full, new
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 126, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A400597:
   squares visited by a chess king moving on a square spiral,
   always moving to the adjacent unvisited square whose number has
   the greatest number of distinct prime factors; ties resolved
   by choosing the lowest number.  The sequence is finite. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

#define MAX_NUM 100000          /* enough to contain all visited squares */
#define R 160                    /* coordinate offset: 2*R+1 = 321  */
#define SIZE (2*R+1)             /* grid dimension */
#define OFFSET (R)

static uint32_t pos_x[MAX_NUM+1];
static uint32_t pos_y[MAX_NUM+1];
static bool      visited[MAX_NUM+1];
static int       grid[SIZE*SIZE];    /* mapping from (x,y) to number, 0 = none */

static seq_t seq_arr[126];
static int   seq_len = 0;
static bool  init_done = false;

/* helper to index 2‑D grid */
static inline int grid_index(int x, int y)
{
    return (x + OFFSET) * SIZE + (y + OFFSET);
}

/* build the spiral mapping up to MAX_NUM */
static void build_spiral(void)
{
    int x = 0, y = 0;
    int dir = 0;   /* 0:right,1:up,2:left,3:down */
    int steps = 1;
    int num = 1;

    grid[grid_index(x, y)] = num;
    pos_x[num] = x; pos_y[num] = y;

    while (num < MAX_NUM) {
        for (int repeat = 0; repeat < 2; ++repeat) {
            for (int i = 0; i < steps && num < MAX_NUM; ++i) {
                /* move one step */
                switch (dir) {
                    case 0: ++x; break;   /* right */
                    case 1: ++y; break;   /* up */
                    case 2: --x; break;   /* left */
                    case 3: --y; break;   /* down */
                }
                ++num;
                grid[grid_index(x, y)] = num;
                pos_x[num] = x; pos_y[num] = y;
            }
            /* turn left */
            dir = (dir + 1) & 3;
        }
        ++steps;
    }
}

/* compute the A400597 sequence once */
static void init_sequence(void)
{
    if (init_done) return;
    /* initialize visited array */
    for (int i = 0; i <= MAX_NUM; ++i) visited[i] = false;

    /* build mapping */
    for (int i = 0; i < SIZE*SIZE; ++i) grid[i] = 0;
    build_spiral();

    /* simulate king */
    uint32_t cur = 1;
    visited[cur] = true;
    seq_arr[0] = (seq_t)cur;
    seq_len = 1;

    while (true) {
        int cx = (int)pos_x[cur];
        int cy = (int)pos_y[cur];
        int bestCnt = -1;
        uint32_t bestNum = 0;

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                int nx = cx + dx;
                int ny = cy + dy;
                if (nx < -OFFSET || nx > OFFSET || ny < -OFFSET || ny > OFFSET) continue;
                int cand = grid[grid_index(nx, ny)];
                if (cand == 0) continue;
                if (visited[cand]) continue;
                int cnt = count_distinct_primes((uint64_t)cand);
                if (cnt > bestCnt || (cnt == bestCnt && cand < bestNum)) {
                    bestCnt = cnt;
                    bestNum = (uint32_t)cand;
                }
            }
        }
        if (bestCnt < 0) break;          /* no more moves */
        cur = bestNum;
        visited[cur] = true;
        seq_arr[seq_len++] = (seq_t)cur;
        if (seq_len >= 126) break;       /* safety, stop at known length */
    }

    init_done = true;
}

seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0) return 0;
    if (n <= (idx_t)seq_len) {
        return seq_arr[(size_t)n - 1];
    }
    /* sequence is finite; return 0 for indices beyond the end */
    return 0;
}
