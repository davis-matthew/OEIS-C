/*
 * A329518  Squares visited by a knight moving on a square-spiral numbered
 *          board where the knight moves to an unvisited square with the
 *          lowest spiral number and with seven or fewer visited neighbors. It
 *          only moves to squares with eight visited neighbors when no other
 *          square is available.
 * https://oeis.org/A329518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, walk
 * Formula:   not stated
 * Range:     u8 n <= 117, u16 n <= 9517, u32 n <= 9517, u64 n <= 9517,
 *            u128 n <= 9517, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simulation of the knight spiral walk (OEIS A329518).
   a(n) is the spiral number of the nth knight‑visited square.
   The knight starts at spiral number 1 and at each step moves to an
   unvisited square with the lowest spiral number that has at most
   seven visited knight neighbours; if no such square exists it may
   move to a square with eight visited neighbours.  The walk stops when no
   legal move is available. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/* External integer types supplied by the build.      */
typedef uint64_t idx_t;   /* index type */

static const int MAX_NUM   = 26000;   /* up to 25809 is enough         */
static const int OFFSET    = 400;     /* radius big enough for 26000   */
static const int SZ        = 2 * OFFSET + 1;

/* Global state – initialized lazily. */
static bool      *visited   = NULL;          /* SZ*SZ bitmap                  */
static int       *num_map   = NULL;          /* spiral number lookup          */
static int       *coord_x   = NULL;          /* coordinates of visited squares*/
static int       *coord_y   = NULL;
static seq_t     *path_seq  = NULL;          /* sequence values               */
static size_t     path_len  = 0;             /* number of visited squares     */

/* Knight move offsets.  */
static const int kdx[8] = {  2,  2, -2, -2,  1,  1, -1, -1 };
static const int kdy[8] = {  1, -1,  1, -1,  2, -2,  2, -2 };

/* Convert (x,y) to a 1‑D index in the visited/num_map arrays.  */
static inline int idx(int x, int y)
{
    return (x + OFFSET) * SZ + (y + OFFSET);
}

/* Initialise all tables once.  */
static void init_tables(void)
{
    if (visited) return;   /* already initialised */

    visited   = (bool*)calloc(SZ * SZ, sizeof(bool));
    num_map   = (int*)  calloc(SZ * SZ, sizeof(int));
    coord_x   = (int*)  malloc(MAX_NUM * sizeof(int));
    coord_y   = (int*)  malloc(MAX_NUM * sizeof(int));
    path_seq  = (seq_t*)malloc(MAX_NUM * sizeof(seq_t));

    /* Generate the square‑spiral mapping up to MAX_NUM.  */
    int curx = 0, cury = 0;
    int n = 1;
    num_map[idx(curx, cury)] = n;
    path_seq[0] = (seq_t)n;
    visited[idx(curx, cury)] = true;
    /* store coordinates for next use */
    coord_x[0] = curx;
    coord_y[0] = cury;
    ++n;

    int step = 1;                              /* current leg length   */
    int dir  = 0;                              /* 0:E,1:N,2:W,3:S      */
    int dirx[4] = { 1, 0, -1, 0 };
    int diry[4] = { 0, 1, 0, -1 };

    while (n <= MAX_NUM)
    {
        for (int repeat = 0; repeat < 2; ++repeat)
        {
            for (int s = 0; s < step && n <= MAX_NUM; ++s)
            {
                curx += dirx[dir];
                cury += diry[dir];
                num_map[idx(curx, cury)] = n;
                /* store coordinates in parallel arrays */
                coord_x[n - 1] = curx;
                coord_y[n - 1] = cury;
                ++n;
            }
            dir = (dir + 1) & 3;
        }
        ++step;
    }

    path_len = 1;      /* we already have the first square */
}

/* Count visited knight neighbours of (x,y). */
static int visited_neighbours(int x, int y)
{
    int cnt = 0;
    for (int m = 0; m < 8; ++m)
    {
        int nx = x + kdx[m];
        int ny = y + kdy[m];
        if (nx < -OFFSET || nx > OFFSET || ny < -OFFSET || ny > OFFSET) continue;
        int index = idx(nx, ny);
        if (visited[index]) ++cnt;
    }
    return cnt;
}

/* Return the nth term of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* outside offset – undefined */

    init_tables();                   /* ensure tables are ready */

    /* Extend simulation until we have at least n terms or the walk ends. */
    while (path_len < (size_t)n)
    {
        int curx = coord_x[path_len - 1];
        int cury = coord_y[path_len - 1];

        int best_num = INT_MAX;
        int best_x = 0, best_y = 0;
        bool found = false;

        /* (1) Look for an unvisited square with <=7 visited neighbours. */
        for (int m = 0; m < 8; ++m)
        {
            int nx = curx + kdx[m];
            int ny = cury + kdy[m];
            if (nx < -OFFSET || nx > OFFSET || ny < -OFFSET || ny > OFFSET) continue;
            int index = idx(nx, ny);
            if (visited[index]) continue;

            int cand_num = num_map[index];
            if (!cand_num) continue;      /* outside precomputed range */

            int neigh = visited_neighbours(nx, ny);
            if (neigh <= 7 && cand_num < best_num)
            {
                best_num = cand_num;
                best_x   = nx;
                best_y   = ny;
                found    = true;
            }
        }

        /* (2) If none, allow movement to a completely trapped square. */
        if (!found)
        {
            for (int m = 0; m < 8; ++m)
            {
                int nx = curx + kdx[m];
                int ny = cury + kdy[m];
                if (nx < -OFFSET || nx > OFFSET || ny < -OFFSET || ny > OFFSET) continue;
                int index = idx(nx, ny);
                if (visited[index]) continue;

                int cand_num = num_map[index];
                if (!cand_num) continue;

                int neigh = visited_neighbours(nx, ny);
                if (neigh == 8 && cand_num < best_num)
                {
                    best_num = cand_num;
                    best_x   = nx;
                    best_y   = ny;
                    found    = true;
                }
            }
        }

        if (!found) break;            /* no legal move – end of walk     */

        /* Accept the chosen move.  */
        visited[idx(best_x, best_y)] = true;
        coord_x[path_len] = best_x;
        coord_y[path_len] = best_y;
        path_seq[path_len] = (seq_t)best_num;
        ++path_len;
    }

    if (n <= (idx_t)path_len)
        return path_seq[n - 1];
    else
        return (seq_t)0;   /* n beyond the finite length of the sequence */
}
