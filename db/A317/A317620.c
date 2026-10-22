/*
 * A317620  Squares visited by a (2,4)-leaper on a spirally numbered board and
 *          moving to the lowest available unvisited square at each step,
 *          squares labelled >=1.
 * https://oeis.org/A317620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = A317619(n-1) + 1 The function simulates a (2,4)-leaper on
 *            a square spiral board. The leaper always moves to the
 *            lowest-numbered unvisited square that is reachable by a
 *            (2,4)-leap. Squares are numbered by the standard Ulam spiral
 *            (see OEIS entry). The sequence is generated on demand and mem...
 * Range:     u8 n <= 13, u16 n <= 2016, u32 n <= 2016, u64 n <= 2016,
 *            u128 n <= 2016, big any n
 * Helpers:   layer_from_num, num_to_coord
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/layer_from_num.h"
#include "../utils/num_to_coord.h"

/* ---------- convert (x,y) -> number on the spiral ---------- */
static seq_t coord_to_num(int64_t x, int64_t y)
{
    int64_t ax = x >= 0 ? x : -x;
    int64_t ay = y >= 0 ? y : -y;
    size_t r = (ax > ay) ? (size_t)ax : (size_t)ay;      /* ring index */

    if (r == 0) return (seq_t)1;

    int64_t r_sgn = (int64_t)r;                          /* signed ring radius */
    seq_t side = ((seq_t)(2 * r));
    seq_t max_num = ((seq_t)(2*r + 1)) * ((seq_t)(2*r + 1));
    seq_t diff;

    if (y == -r_sgn && x > -r_sgn) {                     /* bottom side (exclude left corner) */
        diff = (seq_t)(r_sgn - x);
    } else if (x == -r_sgn) {                            /* left side */
        diff = side + (seq_t)(y + r_sgn);
    } else if (y == r_sgn) {                             /* top side */
        diff = 2*side + (seq_t)(x + r_sgn);
    } else {                                            /* right side */
        diff = 3*side + (seq_t)(r_sgn - y);
    }

    return max_num - diff;
}

/* ---------- static memoization storage ---------- */
static seq_t *visited = NULL;
static size_t visited_len = 0;
static size_t visited_cap = 0;

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                            /* offset 1; unused case */

    /* initialise memoised list with the first square */
    if (visited_len == 0) {
        visited_cap = 16;
        visited = (seq_t*)malloc(visited_cap * sizeof(seq_t));
        visited[0] = (seq_t)1;
        visited_len = 1;
    }

    /* extend the path up to the requested length */
    while (visited_len < (size_t)n) {

        /* coordinate of the current (last visited) square */
        int64_t curr_x, curr_y;
        num_to_coord(visited[visited_len - 1], &curr_x, &curr_y);

        /* search for the lowest-numbered unvisited neighbour */
        seq_t best = 0;        /* placeholder */
        bool found = false;

        static const int movex[8] = {  2,  2, -2, -2,  4,  4, -4, -4};
        static const int movey[8] = {  4, -4,  4, -4,  2, -2,  2, -2};

        for (int k = 0; k < 8; ++k) {
            int64_t nx = curr_x + movex[k];
            int64_t ny = curr_y + movey[k];
            seq_t cand = coord_to_num(nx, ny);

            /* check if cand is already visited */
            bool seen = false;
            for (size_t j = 0; j < visited_len; ++j)
                if (visited[j] == cand) { seen = true; break; }

            if (!seen) {
                if (!found || cand < best) {
                    best = cand;
                    found = true;
                }
            }
        }

        /* append the chosen square */
        if (!found) return 0;   /* should never happen */

        if (visited_len == visited_cap) {
            visited_cap = visited_cap ? visited_cap * 2 : 16;
            visited = (seq_t*)realloc(visited, visited_cap * sizeof(seq_t));
        }
        visited[visited_len++] = best;
    }

    return visited[n - 1];
}
