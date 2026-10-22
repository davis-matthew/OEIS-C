/*
 * A336038  Squares visited by a chess king on a square-spiral numbered board
 *          and stepping to the lowest unvisited adjacent square, where each
 *          step is not in the same direction as the previous step.
 * https://oeis.org/A336038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, walk
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search implementation of sequence A336038: simulate a king walk on a square‑spiral board, always moving to the lowest numbered adjacent unvisited square while never repeating the previous direction. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Global state for memoisation */
static seq_t *seq = NULL;                 /* cached sequence terms      */
static size_t seq_capacity = 0;
static size_t seq_len = 0;

static bool *visited = NULL;              /* visited squares            */
static size_t visited_capacity = 0;

static int64_t curr_x = 0;                /* current king position      */
static int64_t curr_y = 0;
static int prev_dir = -1;                 /* previous direction index   */

static const int dir_dx[8] = { 1, 1, 0,-1,-1,-1, 0, 1};
static const int dir_dy[8] = { 0, 1, 1, 1, 0,-1,-1,-1};

/* Ensure visited array can index up to idx (inclusive). */
static void ensure_visited_capacity(size_t idx)
{
    if (idx < visited_capacity)
        return;
    size_t new_cap = visited_capacity > 0 ? visited_capacity * 2 : 128;
    if (new_cap <= idx)
        new_cap = idx + 1;
    bool *new_visited = (bool *)realloc(visited, new_cap * sizeof(bool));
    if (!new_visited)
        exit(EXIT_FAILURE);
    for (size_t i = visited_capacity; i < new_cap; ++i)
        new_visited[i] = false;
    visited = new_visited;
    visited_capacity = new_cap;
}

/* Ensure seq array can hold at least len elements. */
static void ensure_seq_capacity(size_t len)
{
    if (len <= seq_capacity)
        return;
    size_t new_cap = seq_capacity > 0 ? seq_capacity * 2 : 64;
    if (new_cap <= len)
        new_cap = len + 1;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_seq)
        exit(EXIT_FAILURE);
    seq = new_seq;
    seq_capacity = new_cap;
}

/* Compute the spiral number for coordinates (x,y).  The spiral starts at (0,0)=1. */
static seq_t
spiral_number(int64_t x, int64_t y)
{
    int64_t ax = x >= 0 ? x : -x;
    int64_t ay = y >= 0 ? y : -y;
    int64_t r = ax > ay ? ax : ay;

    if (r == 0)
        return (seq_t)1;

    int64_t side = 2 * r;                 /* side length of ring   */
    seq_t maxnum = (seq_t)(2 * r + 1) * (seq_t)(2 * r + 1);
    seq_t diff;

    if (y == -r) {            /* bottom edge, moving left */
        diff = (seq_t)(r - x);
    } else if (x == -r) {     /* left edge, moving up */
        diff = (seq_t)(side + (y + r));
    } else if (y == r) {      /* top edge, moving right */
        diff = (seq_t)(2 * side + (x + r));
    } else {                  /* right edge, moving down */
        diff = (seq_t)(3 * side + (r - y));
    }

    return maxnum - diff;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined below offset */

    /* Initialise on first call */
    if (seq_len == 0) {
        /* First term */
        ensure_seq_capacity(1);
        seq[0] = (seq_t)1;
        seq_len = 1;

        /* Mark the starting square as visited */
        ensure_visited_capacity(1);
        visited[1] = true;

        curr_x = 0;
        curr_y = 0;
        prev_dir = -1;
    }

    /* Simulate the walk until we have at least n terms (or no legal moves remain). */
    while (seq_len < (size_t)n) {
        seq_t best_val = (seq_t)-1;      /* sentinel value */
        int best_dir = -1;

        for (int d = 0; d < 8; ++d) {
            if (d == prev_dir)
                continue;               /* cannot repeat previous direction */

            int64_t nx = curr_x + dir_dx[d];
            int64_t ny = curr_y + dir_dy[d];
            seq_t val = spiral_number(nx, ny);

            /* Ensure visited array can index val */
            if ((size_t)val >= visited_capacity)
                ensure_visited_capacity((size_t)val + 1);

            if (visited[(size_t)val])
                continue;               /* already visited */

            if (best_dir == -1 || val < best_val) {
                best_val = val;
                best_dir = d;
            }
        }

        if (best_dir == -1)            /* no legal move */
            break;

        /* Move king */
        curr_x += dir_dx[best_dir];
        curr_y += dir_dy[best_dir];
        prev_dir = best_dir;

        /* Record new square */
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len] = best_val;

        /* Mark as visited */
        if ((size_t)best_val >= visited_capacity)
            ensure_visited_capacity((size_t)best_val + 1);
        visited[(size_t)best_val] = true;

        seq_len++;
    }

    if ((size_t)n - 1 < seq_len)
        return seq[n - 1];
    return (seq_t)0;
}
