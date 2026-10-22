/*
 * A323808  Squares visited by a knight on a spirally numbered board and
 *          moving to the lowest available unvisited square at each step and
 *          if no unvisited squares are available move one step back.
 * https://oeis.org/A323808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = A323809(n-1) + 1 (continuous extension)
 * Range:     u8 n <= 117, u16 n <= 9629, u32 n <= 9629, u64 n <= 9629,
 *            u128 n <= 9629, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Knight step offsets */
static const long long KD[8] = { 1,  1,  2,  2, -1, -1, -2, -2 };
static const long long KE[8] = { 2, -2,  1, -1,  2, -2,  1, -1 };

/* Dynamic storage for precomputed terms */
static seq_t *seq_vals = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

/* Dynamic stack of visited coordinates (path) */
static long long *path_x = NULL;
static long long *path_y = NULL;
static size_t path_cap = 0;
static size_t path_len = 0;

/* Visited flag array indexed by board number */
static bool *visited = NULL;
static size_t vis_cap = 0;

static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1024;
    while (new_cap < need) new_cap *= 2;
    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void ensure_path_cap(size_t need)
{
    if (need <= path_cap) return;
    size_t new_cap = path_cap ? path_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    path_x = (long long *)realloc(path_x, new_cap * sizeof(long long));
    path_y = (long long *)realloc(path_y, new_cap * sizeof(long long));
    path_cap = new_cap;
}

static void ensure_vis_cap(size_t need)
{
    if (need < vis_cap) return;
    size_t new_cap = vis_cap ? vis_cap : 8;
    while (new_cap <= need) new_cap <<= 1;
    visited = (bool *)realloc(visited, new_cap * sizeof(bool));
    /* new entries are already zeroed by realloc on most systems; clear just in case */
    for (size_t i = vis_cap; i < new_cap; ++i) visited[i] = false;
    vis_cap = new_cap;
}

/* Spiral numbering (Ulam spiral) */
static seq_t board_number(long long x, long long y)
{
    long long ax = x >= 0 ? x : -x;
    long long ay = y >= 0 ? y : -y;
    long long L = ax > ay ? ax : ay;

    if (L == 0) return 1;

    seq_t min_num = (seq_t)((2 * L - 1) * (2 * L - 1) + 1);
    seq_t offset = 0;

    if (x == L && y >= -L + 1 && y <= L) {
        offset = (seq_t)(y + L - 1);
    } else if (y == L && x >= -L && x <= L - 1) {
        offset = (seq_t)(2 * L + (L - 1 - x));
    } else if (x == -L && y >= -L && y <= L - 1) {
        offset = (seq_t)(4 * L + (L - 1 - y));
    } else { /* y == -L */
        offset = (seq_t)(6 * L + (x + L - 1));
    }
    return min_num + offset;
}

/* Lazy initialization of the sequence */
static void init_sequence(void)
{
    if (seq_len > 0) return;          /* already initialized */
    /* initial capacities */
    seq_cap = 1024;
    seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));

    path_cap = 64;
    path_x = (long long *)malloc(path_cap * sizeof(long long));
    path_y = (long long *)malloc(path_cap * sizeof(long long));

    vis_cap = 8;
    visited = (bool *)malloc(vis_cap * sizeof(bool));
    for (size_t i = 0; i < vis_cap; ++i) visited[i] = false;

    /* starting point (0,0) -> board number 1 */
    seq_len = 1;
    seq_vals[0] = 1;

    path_len = 1;
    path_x[0] = 0;
    path_y[0] = 0;

    ensure_vis_cap(1);
    visited[1] = true;
}

/* Main function requested by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of defined range, safe return */

    init_sequence();

    /* compute further terms if necessary */
    while (seq_len < n) {
        bool found = false;
        seq_t best_num = 0;
        long long bestx = 0, besty = 0;

        long long curx = path_x[path_len - 1];
        long long cury = path_y[path_len - 1];

        /* search for lowest unvisited reachable square */
        for (int i = 0; i < 8; ++i) {
            long long nx = curx + KD[i];
            long long ny = cury + KE[i];
            seq_t num = board_number(nx, ny);
            size_t idx = (size_t)num;
            if (idx >= vis_cap) ensure_vis_cap(idx);
            if (!visited[idx]) {
                if (!found || num < best_num) {
                    found = true;
                    best_num = num;
                    bestx = nx;
                    besty = ny;
                }
            }
        }

        if (found) {
            /* move to new square */
            ensure_path_cap(path_len + 1);
            path_x[path_len] = bestx;
            path_y[path_len] = besty;
            ++path_len;

            ensure_vis_cap((size_t)best_num);
            visited[(size_t)best_num] = true;

            ensure_seq_cap(seq_len + 1);
            seq_vals[seq_len] = best_num;
            ++seq_len;
        } else {
            /* backtrack one step (if possible) */
            if (path_len >= 2) {
                --path_len;            /* remove current position */
                long long bx = path_x[path_len - 1];
                long long by = path_y[path_len - 1];
                seq_t num = board_number(bx, by);
                ensure_seq_cap(seq_len + 1);
                seq_vals[seq_len] = num;
                ++seq_len;
                /* visited flag already set */
            } else {
                /* at the origin, no backtrack possible – repeat the first square */
                seq_t num = 1;
                ensure_seq_cap(seq_len + 1);
                seq_vals[seq_len] = num;
                ++seq_len;
            }
        }
    }

    return seq_vals[n - 1];
}
