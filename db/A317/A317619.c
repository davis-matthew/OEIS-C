/*
 * A317619  Squares visited by a (2,4)-leaper on a spirally numbered board and
 *          moving to the lowest available unvisited square at each step,
 *          squares labelled >=0.
 * https://oeis.org/A317619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 2015, u32 n <= 2015, u64 n <= 2015,
 *            u128 n <= 2015, big any n
 * Helpers:   coords_to_num, max_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A317619: Squares visited by a (2,4)-leaper on a spirally numbered board
   moving to the lowest available unvisited square.
   Formula: a(n) = A317620(n+1) - 1
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>   /* for abs */
#include "../utils/coords_to_num.h"
#include "../utils/max_int.h"

/* ----------------------------------------------------------------------
   State: sequence and visited bitset                              */
static seq_t *seq = NULL;      /* sequence values a(0)..a(len-1) */
static size_t seq_len = 0, seq_cap = 0;

static uint8_t *visited = NULL;    /* visited[number] == 1 if visited   */
static size_t visited_cap = 0;

static void ensure_seq_cap(idx_t n)
{
    if (n < seq_len) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap <= (size_t)n + 1) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
    /* new area is uninitialized; values will be written later */
}

static void ensure_visited_cap(seq_t n)
{
    size_t idx = (size_t)n;
    if (idx < visited_cap) return;
    size_t new_cap = visited_cap ? visited_cap : 1024;
    while (new_cap <= idx) new_cap <<= 1;
    visited = (uint8_t *)realloc(visited, new_cap);
    memset(visited + visited_cap, 0, new_cap - visited_cap);
    visited_cap = new_cap;
}

/* ----------------------------------------------------------------------
   exported function                                                */
seq_t a(idx_t n)
{
    /* initialise if first call */
    if (seq_len == 0) {
        ensure_seq_cap(0);
        seq[0] = 0;
        seq_len = 1;
        ensure_visited_cap(0);
        visited[0] = 1;
    }

    /* current state; kept static across calls */
    static int cur_x = 0, cur_y = 0;
    static seq_t cur_num = 0;

    /* leaper move offsets */
    static const int moves[8][2] = {
        { 2,  4}, { 2, -4}, {-2,  4}, {-2, -4},
        { 4,  2}, { 4, -2}, {-4,  2}, {-4, -2}
    };

    /* extend sequence until we have a(n) */
    while (seq_len <= n) {

        seq_t best_num = (seq_t)(-1);  /* highest possible value */
        int best_dx = 0, best_dy = 0;

        for (int m = 0; m < 8; ++m) {
            int tx = cur_x + moves[m][0];
            int ty = cur_y + moves[m][1];
            seq_t tnum = coords_to_num(tx, ty);
            ensure_visited_cap(tnum);
            if (!visited[(size_t)tnum]) {
                if (best_num == (seq_t)(-1) || tnum < best_num) {
                    best_num = tnum;
                    best_dx = moves[m][0];
                    best_dy = moves[m][1];
                }
            }
        }

        /* move to chosen square */
        cur_x += best_dx;
        cur_y += best_dy;
        cur_num = best_num;

        visited[(size_t)cur_num] = 1;
        ensure_seq_cap(seq_len);
        seq[seq_len++] = cur_num;
    }

    return seq[n];
}
