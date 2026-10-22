/*
 * A362027  Squares visited by a knight moving on a square-spiral numbered
 *          board where the knight moves to a previously unvisited square with
 *          a number as close as possible to the number of the current square.
 *          If two such squares exist the smaller numbered square is chosen.
 * https://oeis.org/A362027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 130, u32 n <= 130, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Helpers:   abs_diff_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A362027: Knight visits on a square‑spiral board.
    The algorithm precomputes the entire finite sequence by
    simulating the knight and selecting the unvisited neighbour
    with the smallest absolute difference to the current square.
    If there is a tie the smaller numbered square is chosen.
*/
#include <stdbool.h>
#include "../utils/abs_diff_seq_t.h"

/* ----- parameters ------------------------------------------------------ */
#define MAX_NUM   800          /* generate spiral numbers up to this value */
#define SHIF      200          /* offset for grid indices (ensures positivity) */
#define GRID_SZ   (2*SHIF+1)   /* 2‑D grid size                          */
#define MAX_SEQ   200          /* enough to hold the entire sequence      */

/* ----- static storage -------------------------------------------------- */
static seq_t  seq_cache[MAX_SEQ];      /* cached sequence values          */
static idx_t  seq_cache_len = 0;       /* number of cached values          */
static bool   visited[MAX_NUM+1] = {0};/* whether number has been visited  */
static int    coord_x[MAX_NUM+1];      /* x‑coordinate of a number        */
static int    coord_y[MAX_NUM+1];      /* y‑coordinate of a number        */
static int    grid[GRID_SZ][GRID_SZ] = {0}; /* number stored at a coordinate */

/* ----- initialise cache ---------------------------------------------- */
static void init_sequence(void)
{
    /* build square‑spiral mapping up to MAX_NUM */
    int x = 0, y = 0;
    int n = 1;
    coord_x[n] = x;
    coord_y[n] = y;
    grid[SHIF + y][SHIF + x] = n;

    int step_len   = 1;   /* current leg length */
    int dir        = 0;   /* 0:right, 1:up, 2:left, 3:down */
    int step_done  = 0;   /* steps taken in current leg   */
    int changes    = 0;   /* number of direction changes */

    while (n < MAX_NUM) {
        /* move one step */
        if (dir == 0)      ++x;
        else if (dir == 1) ++y;
        else if (dir == 2) --x;
        else               --y;

        /* record new number */
        ++n;
        coord_x[n] = x;
        coord_y[n] = y;
        grid[SHIF + y][SHIF + x] = n;

        /* update leg progress */
        ++step_done;
        if (step_done == step_len) {
            dir = (dir + 1) & 3;
            step_done = 0;
            ++changes;
            if ((changes & 1) == 0)  /* increase leg length every two turns */
                ++step_len;
        }
    }

    /* Knight walk simulation */
    static const int dx[8] = { 2,  2,  1,  1, -1, -1, -2, -2};
    static const int dy[8] = { 1, -1,  2, -2,  2, -2,  1, -1};

    int current = 1;          /* start at square 1 */
    visited[1] = true;
    seq_cache[0] = (seq_t)1;
    seq_cache_len = 1;

    while (true) {
        seq_t best_diff = 0;
        int   best_num  = 0;
        bool  found     = false;

        int cx = coord_x[current];
        int cy = coord_y[current];

        for (int m = 0; m < 8; ++m) {
            int nx = cx + dx[m];
            int ny = cy + dy[m];
            if (nx < -SHIF || nx > SHIF || ny < -SHIF || ny > SHIF)
                continue;                    /* out of mapped grid */
            int num = grid[SHIF + ny][SHIF + nx];
            if (num == 0 || visited[num])
                continue;                    /* not numbered or already visited */

            seq_t diff = abs_diff_seq_t((seq_t)num, (seq_t)current);
            if (!found || diff < best_diff ||
                (diff == best_diff && num < best_num)) {
                found = true;
                best_diff = diff;
                best_num  = num;
            }
        }

        if (!found)                     /* no legal move – terminate */
            break;

        visited[best_num] = true;
        seq_cache[seq_cache_len++] = (seq_t)best_num;
        current = best_num;
    }
}

/* ----- public function ------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_sequence();
        initialized = true;
    }

    /* Sequence is defined for n ≥ 1 up to seq_cache_len; return 0 otherwise */
    if (n == 0 || n > seq_cache_len)
        return (seq_t)0;

    return seq_cache[n - 1];
}
