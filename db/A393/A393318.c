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
 * Helpers:   spiral_coords_from_num, spiral_num_from_coords, spiral_num_to_x,
 *            spiral_num_to_y
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
#include "../utils/spiral_coords_from_num.h"
#include "../utils/spiral_num_from_coords.h"
#include "../utils/spiral_num_to_x.h"
#include "../utils/spiral_num_to_y.h"

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
