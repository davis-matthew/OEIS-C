/*
 * A361377  Squares visited by a knight moving on a spirally numbered board
 *          always to the lowest unvisited coprime square.
 * https://oeis.org/A361377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 156, u32 n <= 156, u64 n <= 156,
 *            u128 n <= 156, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A361377: squares visited by a knight on a spirally numbered board,
   always moving to the lowest unvisited coprime square.  The sequence
   is finite (156 terms).  The implementation uses a lazy, memoized
   simulation with a knigh‑t move table and a spiral number mapping.
   The code is fully portable to C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"

/* Convert (x,y) coordinates on the Ulam spiral to the board number. */
static seq_t coord_to_num(long long x, long long y)
{
    long long ax = x >= 0 ? x : -x;
    long long ay = y >= 0 ? y : -y;
    long long r  = ax > ay ? ax : ay;
    seq_t max_num = (seq_t)(2 * r + 1);
    max_num = max_num * max_num;                 /* (2r+1)^2 */
    seq_t rs = (seq_t)r;
    if (y == -r) {                               /* bottom side, rightward */
        return max_num - (rs - (seq_t)x);
    } else if (x == -r) {                        /* left side, upward */
        return max_num - ((seq_t)(2 * r) + (rs + (seq_t)y));
    } else if (y == r) {                         /* top side, leftward */
        return max_num - ((seq_t)(4 * r) + (rs + (seq_t)x));
    } else {                                     /* right side, downward */
        return max_num - ((seq_t)(6 * r) + (rs - (seq_t)y));
    }
}

/* dynamic storage for the computed sequence */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0, seq_cap = 0;

/* visited flags for numbers that have appeared */
static bool *visited = NULL;
static size_t visited_cap = 0;

/* current position in the simulation */
static long long cur_x = 0, cur_y = 0;
static seq_t cur_num = 0;
static bool initialized = false;

/* ensure the visited array is large enough to store flag for 'n' */
static void ensure_visited_capacity(seq_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= visited_cap)
        return;
    size_t new_cap = visited_cap ? visited_cap : 1024;
    while (new_cap < needed)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(visited, new_cap * sizeof(bool));
    if (!tmp)
        exit(1);
    memset(tmp + visited_cap, 0, (new_cap - visited_cap) * sizeof(bool));
    visited = tmp;
    visited_cap = new_cap;
}

/* ensure the sequence array can hold at least 'need' entries */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    seq_arr = tmp;
    seq_cap = new_cap;
}

/* initialise the walk once */
static void init_sequence(void)
{
    if (initialized)
        return;
    initialized = true;
    ensure_seq_capacity(16);
    seq_arr[0] = 1;
    seq_len = 1;
    cur_x = 0;
    cur_y = 0;
    cur_num = 1;
    ensure_visited_capacity(1);
    visited[1] = true;
}

/* Knight move deltas */
static const long long dx[8] = { 1,  1, -1, -1,  2,  2, -2, -2 };
static const long long dy[8] = { 2, -2,  2, -2,  1, -1,  1, -1 };

/* generate the sequence up to at least 'up_to' terms */
static void generate_next_terms(size_t up_to)
{
    init_sequence();
    while (seq_len < up_to) {
        seq_t best = 0;
        long long best_x = 0, best_y = 0;
        for (int i = 0; i < 8; ++i) {
            long long nx = cur_x + dx[i];
            long long ny = cur_y + dy[i];
            seq_t num = coord_to_num(nx, ny);
            ensure_visited_capacity(num);
            if (visited[(size_t)num])
                continue;
            if (gcd(cur_num, num) != (seq_t)1)
                continue;
            if (best == 0 || num < best) {
                best = num;
                best_x = nx;
                best_y = ny;
            }
        }
        if (best == 0)                     /* no unvisited coprime move */
            break;
        ensure_seq_capacity(seq_len + 1);
        seq_arr[seq_len++] = best;
        ensure_visited_capacity(best);
        visited[(size_t)best] = true;
        cur_x = best_x;
        cur_y = best_y;
        cur_num = best;
    }
}

/* external sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) undefined, return 0 as a sentinel */
        return 0;
    generate_next_terms((size_t)n);
    if (n <= seq_len)
        return seq_arr[(size_t)n - 1];
    return 0;               /* sequence terminates before n */
}
