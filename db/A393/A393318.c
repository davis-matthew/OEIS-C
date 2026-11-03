/*
 * A393318  Final square visited by a knight moving on a square-spiral
 *          numbered board when starting from square n and always moving to
 *          the lowest available unvisited square.
 * https://oeis.org/A393318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9121 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: simulation of a knight moving on a square‑spiral board starting
   from square number n, always moving to the lowest numbered unvisited square.
   Returns the final square visited. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int64_t spiral_num_to_x(int64_t k, int64_t offset)
{
    /* helper to compute x coordinate on the layer k */
    if (offset < 2 * k)       /* right side (excluding corner) */
        return k;
    if (offset < 4 * k)       /* top side */
        return k - 1 - (offset - 2 * k);
    if (offset < 6 * k)       /* left side */
        return -k;
    /* bottom side (including corner) */
    return -k + 1 + (offset - 6 * k);
}

static int64_t spiral_num_to_y(int64_t k, int64_t offset)
{
    if (offset < 2 * k)       /* right side (excluding corner) */
        return -k + 1 + offset;
    if (offset < 4 * k)       /* top side */
        return k;
    if (offset < 6 * k)       /* left side */
        return k - 1 - (offset - 4 * k);
    /* bottom side (including corner) */
    return -k;
}

/* Convert board number to (x,y) coordinates on the square spiral */
static void spiral_coords_from_num(uint64_t num, int64_t *x, int64_t *y)
{
    if (num == 1) {
        *x = 0;
        *y = 0;
        return;
    }
    uint64_t k = 0;
    while ((2 * k + 1) * (2 * k + 1) < num) ++k;   /* layer of num */
    uint64_t side  = 2 * k + 1;
    uint64_t prev  = (side - 2) * (side - 2);
    uint64_t offset = num - prev - 1;              /* 0‑based offset on the ring */
    *x = spiral_num_to_x(k, offset);
    *y = spiral_num_to_y(k, offset);
}

/* Convert (x,y) coordinates back to board number */
static uint64_t spiral_num_from_coords(int64_t x, int64_t y)
{
    int64_t ax = x >= 0 ? x : -x;
    int64_t ay = y >= 0 ? y : -y;
    int64_t k  = ax > ay ? ax : ay;                 /* layer */
    if (k == 0) return 1;

    uint64_t kk   = (uint64_t)k;
    uint64_t side = 2 * kk + 1;
    uint64_t prev = (side - 2) * (side - 2);        /* last number of inner layer */

    uint64_t offset;
    if (x ==   k && y != -k)                    /* right side, not corner */
        offset = (uint64_t)(y + k - 1);
    else if (y ==   k)                          /* top side */
        offset = 2 * kk + ((kk - 1) - (uint64_t)x);
    else if (x == -k)                           /* left side */
        offset = 4 * kk + ((kk - 1) - (uint64_t)y);
    else                                        /* bottom side, includes corner (k,-k) */
        offset = 6 * kk + ((uint64_t)x + k - 1);

    return prev + 1 + offset;
}

/*---------- square–spiral conversion ------------------*/

/*---------- main sequence function -------------------*/

#define VISITED_CAP 2000000 /* must be constant expression for static array size */

static uint32_t visited_gen[VISITED_CAP];
static uint32_t cur_gen = 1;

seq_t a(idx_t n)
{
    /* check for generation counter overflow */
    if (cur_gen == 0) {
        memset(visited_gen, 0, sizeof(visited_gen));
        cur_gen = 1;
    }
    uint32_t this_gen = cur_gen++;

    /* initial position */
    uint64_t cur = (uint64_t)n;
    if (cur >= VISITED_CAP) return (seq_t)0;          /* safety guard */

    visited_gen[(size_t)cur] = this_gen;

    /* knight move offsets */
    static const int64_t dx[8] = { 1,  2,  2,  1, -1, -2, -2, -1 };
    static const int64_t dy[8] = { 2,  1, -1, -2, -2, -1,  1,  2 };

    while (1) {
        int64_t x, y;
        spiral_coords_from_num(cur, &x, &y);

        uint64_t best = 0;
        bool found = false;

        for (int i = 0; i < 8; ++i) {
            int64_t nx = x + dx[i];
            int64_t ny = y + dy[i];
            uint64_t nxt = spiral_num_from_coords(nx, ny);
            if (nxt >= VISITED_CAP) continue;       /* out of static range */
            if (visited_gen[(size_t)nxt] != this_gen) {
                if (!found || nxt < best) {
                    best = nxt;
                    found = true;
                }
            }
        }

        if (!found) {               /* no unvisited move left */
            return (seq_t)cur;
        }

        /* move to the chosen square */
        visited_gen[(size_t)best] = this_gen;
        cur = best;
    }
}
