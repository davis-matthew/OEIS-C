/*
 * A317415  Squares visited by a (1,4)-leaper (or giraffe) on a spirally
 *          numbered board and moving to the lowest available unvisited square
 *          at each step, squares labelled >=0.
 * https://oeis.org/A317415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = A317416(n+1) - 1 where A317416 gives the number of the
 *            square visited by a (1,4)-leaper moving to the lowest numbered
 *            unvisited square, starting from square 0
 * Range:     u8 n <= 112, u16 n <= 9561, u32 n <= 9561, u64 n <= 9561,
 *            u128 n <= 9561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <new>
#endif

/* 8 possible (1,4)-leaper moves */
static const int dx[8] = {  1,  1, -1, -1,  4,  4, -4, -4};
static const int dy[8] = {  4, -4,  4, -4,  1, -1,  1, -1};

/* convert (x,y) to spiral number; coordinates are 64‑bit signed */
static idx_t coord_to_num(int64_t x, int64_t y)
{
    if (x == 0 && y == 0) return 0;

    int64_t absx = x >= 0 ? x : -x;
    int64_t absy = y >= 0 ? y : -y;
    int64_t k = absx > absy ? absx : absy;           /* ring index */

    /* first number on this ring */
    idx_t nmin = (idx_t)((2 * (k - 1) + 1) * (2 * (k - 1) + 1));

    int64_t offset;
    if (x == k && y >= -(k - 1) && y <= k) {
        /* right side, moving up */
        offset = y + (k - 1);
    } else if (y == k && x <= k - 1 && x >= -k) {
        /* top side, moving left */
        offset = 2 * k + ((k - 1) - x);
    } else if (x == -k && y <= k - 1 && y >= -k) {
        /* left side, moving down */
        offset = 4 * k + ((k - 1) - y);
    } else {
        /* bottom side, moving right */
        offset = 6 * k + (x + k - 1);
    }

    return nmin + (idx_t)offset;
}

/* visited squares; grows as necessary */
static uint8_t *visited = NULL;
static size_t visited_cap = 0;

static void ensure_visited_cap(idx_t need)
{
    if (need < visited_cap) return;
    size_t newcap = visited_cap ? visited_cap : 256;
    while (newcap <= (size_t)need) newcap <<= 1;
    uint8_t *newptr = (uint8_t *)realloc(visited, newcap * sizeof(uint8_t));
    if (!newptr) { /* handle out of memory */ abort(); }
    memset(newptr + visited_cap, 0, (newcap - visited_cap) * sizeof(uint8_t));
    visited = newptr;
    visited_cap = newcap;
}

/* sequence buffer – fixed maximum size sufficient for n <= 10000 */
#ifdef __cplusplus
static seq_t *seq = nullptr;
static const size_t seq_cap = 1 << 18;      /* 262 144 slots */
#else
static seq_t *seq = NULL;
static const size_t seq_cap = 1 << 18;
#endif
static size_t seq_len = 0;

/* current leaper position */
static int64_t cur_x = 0, cur_y = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
#if defined(__cplusplus)
        seq = new seq_t[seq_cap];
#else
        seq = (seq_t *)calloc(seq_cap, sizeof(seq_t));
#endif
        if (!seq) { abort(); }

        visited = (uint8_t *)calloc(256, sizeof(uint8_t));
        if (!visited) { abort(); }
        visited_cap = 256;

        /* start at square 0 */
        ensure_visited_cap(0);
        visited[0] = 1;
        seq[0] = 0;
        seq_len = 1;
        cur_x = 0;
        cur_y = 0;

        initialized = true;
    }

    while (seq_len <= (size_t)n) {
        idx_t best_num = (idx_t)-1;
        int64_t best_x = cur_x, best_y = cur_y;   /* defaults */

        /* examine all possible moves */
        for (int dir = 0; dir < 8; ++dir) {
            int64_t nx = cur_x + dx[dir];
            int64_t ny = cur_y + dy[dir];
            idx_t num = coord_to_num(nx, ny);

            ensure_visited_cap(num);
            if (!visited[num]) {
                if (num < best_num) {
                    best_num = num;
                    best_x = nx;
                    best_y = ny;
                }
            }
        }

        /* mark chosen square as visited */
        ensure_visited_cap(best_num);
        visited[best_num] = 1;

        /* store the result; cast to seq_t */
        seq[seq_len++] = (seq_t)best_num;

        /* advance position */
        cur_x = best_x;
        cur_y = best_y;
    }

    return seq[n];
}
