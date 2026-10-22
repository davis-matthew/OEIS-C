/*
 * A343388  Squares visited by a knight moving on a square-spiral with numbers
 *          equal to the ordered divisors of the positive integers and where
 *          the knight moves to the smallest numbered unvisited square; the
 *          minimum distance from the origin is used if the square numbers are
 *          equal; the smallest ordered spiral...
 * https://oeis.org/A343388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   index_from_coord
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Simulate knight moves on the ordered‑divisor square spiral.
   The function caches the divisor values indexed by spiral position and
   keeps a list of visited coordinates to choose the next move according to
   the rules described in the OEIS entry. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>   /* for SIZE_MAX */
#include "../utils/index_from_coord.h"

/* ------------ ordered‑divisor sequence cache ------------ */
static seq_t *div_seq   = NULL;   /* values in spiral order   */
static size_t div_len   = 0;      /* number of values stored  */
static size_t div_cap   = 0;      /* allocated capacity       */
static uint64_t next_m  = 1;      /* next integer to process */

/* Ensure div_seq contains at least the first (idx+1) values. */
static void ensure_div_seq_upto(size_t idx)
{
    while (div_len <= idx) {
        uint64_t m = next_m++;
        uint64_t root = 0;
        while ((root + 1) * (root + 1) <= m) ++root;

        uint64_t small[512];
        uint64_t large[512];
        size_t cnt_small = 0, cnt_large = 0;

        for (uint64_t d = 1; d <= root; ++d) {
            if (m % d == 0) {
                small[cnt_small++] = d;
                if (d * d != m)
                    large[cnt_large++] = m / d;
            }
        }

        if (div_len + cnt_small + cnt_large > div_cap) {
            div_cap = div_cap ? div_cap * 2 : 256;
            if (div_cap < div_len + cnt_small + cnt_large)
                div_cap = div_len + cnt_small + cnt_large;
            div_seq = (seq_t *)realloc(div_seq, div_cap * sizeof(seq_t));
        }

        for (size_t i = 0; i < cnt_small; ++i)
            div_seq[div_len++] = (seq_t)small[i];

        for (size_t i = 0; i < cnt_large; ++i)
            div_seq[div_len++] = (seq_t)large[cnt_large - 1 - i];
    }
}

/* ------------ visited coordinates ------------ */
typedef struct { int x; int y; } VisCoord;

static VisCoord *visited   = NULL;
static size_t visited_len  = 0;
static size_t visited_cap  = 0;

static bool visited_contains(int x, int y)
{
    for (size_t i = 0; i < visited_len; ++i)
        if (visited[i].x == x && visited[i].y == y)
            return true;
    return false;
}

static void visited_add(int x, int y)
{
    if (visited_len == visited_cap) {
        visited_cap = visited_cap ? visited_cap * 2 : 8;
        visited = (VisCoord *)realloc(visited, visited_cap * sizeof(VisCoord));
    }
    visited[visited_len++] = (VisCoord){x, y};
}

/* ------------ main sequence function ------------ */
seq_t a(idx_t n)
{
    /* initialise static storage once */
    static bool initialized = false;
    if (!initialized) {
        initialized = true;

        visited_cap = 8;
        visited = (VisCoord *)malloc(visited_cap * sizeof(VisCoord));
        visited_len = 0;

        div_cap = 256;
        div_seq = (seq_t *)malloc(div_cap * sizeof(seq_t));
        div_len = 0;
        next_m = 1;
        ensure_div_seq_upto(0);     /* origin value */
    }

    /* reset for this call */
    visited_len = 0;

    /* starting position (origin) */
    int cx = 0, cy = 0;
    visited_add(cx, cy);
    seq_t last_val = div_seq[0];

    if (n <= 1) return last_val;

    size_t step = 1;   /* number of visited squares so far */
    const int moves[8][2] = {
        {  2,  1}, {  1,  2}, { -1,  2}, { -2,  1},
        { -2, -1}, { -1, -2}, {  1, -2}, {  2, -1}
    };

    while (step < n) {
        size_t best_idx   = SIZE_MAX;
        seq_t  best_val   = 0;
        long long best_dist = 0;
        size_t best_order  = SIZE_MAX;
        int best_x = 0, best_y = 0;

        for (int k = 0; k < 8; ++k) {
            int nx = cx + moves[k][0];
            int ny = cy + moves[k][1];
            if (visited_contains(nx, ny))
                continue;

            size_t idx = index_from_coord(nx, ny);
            ensure_div_seq_upto(idx);
            seq_t val = div_seq[idx];

            long long dist = (long long)nx * nx + (long long)ny * ny;
            size_t order = idx;   /* spiral index (0‑based) */

            if (best_idx == SIZE_MAX ||
                val < best_val ||
                (val == best_val && (
                    dist < best_dist ||
                    (dist == best_dist && order < best_order)))) {
                best_idx = idx;
                best_val = val;
                best_dist = dist;
                best_order = order;
                best_x = nx;
                best_y = ny;
            }
        }

        if (best_idx == SIZE_MAX) {
            /* no unvisited adjacent square – sequence ends */
            return last_val;
        }

        cx = best_x;
        cy = best_y;
        visited_add(cx, cy);
        last_val = div_seq[best_idx];
        ++step;
    }

    return last_val;
}
