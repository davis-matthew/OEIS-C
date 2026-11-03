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
 * Validated: full, pass, 2016 terms through n = 2016 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* ---------- helper: layer (ring) containing number n ---------- */
static size_t layer_from_num(seq_t n)
{
    if (n <= (seq_t)1) return 0;          /* center */

    size_t low = 0, high = 1;
    seq_t max_num;

    /* find high enough so that (2*high+1)^2 >= n */
    while (1) {
        max_num = ((seq_t)(2*high + 1)) * ((seq_t)(2*high + 1));
        if (max_num >= n) break;
        high <<= 1;                       /* double high */
    }

    /* binary search for minimal r */
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        seq_t mid_max = ((seq_t)(2*mid + 1)) * ((seq_t)(2*mid + 1));
        if (mid_max >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* ---------- convert number -> (x,y) on the spiral ---------- */
static void num_to_coord(seq_t n, int64_t *x, int64_t *y)
{
    if (n == (seq_t)1) { *x = 0; *y = 0; return; }

    size_t r = layer_from_num(n);          /* ring index */
    int64_t r_sgn = (int64_t)r;            /* signed for calculations */

    seq_t side = ((seq_t)(2 * r));         /* 2r  */
    seq_t max_num = ((seq_t)(2*r + 1)) * ((seq_t)(2*r + 1));
    seq_t diff = max_num - n;              /* distance from max */

    size_t diff_us = (size_t)diff;
    size_t side_us = (size_t)side;

    if (diff_us < side_us) {               /* bottom side, excluding left corner */
        *x = r_sgn - (int64_t)diff_us;
        *y = -r_sgn;
    } else if (diff_us < 2*side_us) {       /* left side */
        size_t d = diff_us - side_us;
        *x = -r_sgn;
        *y = -r_sgn + (int64_t)d;
    } else if (diff_us < 3*side_us) {       /* top side */
        size_t d = diff_us - 2*side_us;
        *x = -r_sgn + (int64_t)d;
        *y = r_sgn;
    } else {                                /* right side */
        size_t d = diff_us - 3*side_us;
        *x = r_sgn;
        *y = r_sgn - (int64_t)d;
    }
}

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
        visited = mem_new(seq_t, visited_cap);
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
            visited = mem_resize(seq_t, visited, visited_cap);
        }
        visited[visited_len++] = best;
    }

    return visited[n - 1];
}
