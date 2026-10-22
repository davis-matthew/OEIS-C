/*
 * A400593  Squares visited by a chess king moving on the square spiral,
 *          always moving to an adjacent unvisited square whose number has the
 *          greatest prime factor; ties are resolved by choosing the lowest
 *          number.
 * https://oeis.org/A400593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, fini, full, new
 * Formula:   a(n) is defined by a walk on a square spiral (see OEIS entry)
 * Range:     u8 n <= 9, u16 n <= 256, u32 n <= 256, u64 n <= 256,
 *            u128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A400593 by simulating the king's walk on the square spiral.
   The sequence is finite; a(256) = 9747.  The function computes all
   terms up to 256 on the first call. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* constants */
#define MAX_NUM   15000              /* maximum number generated in the spiral   */
#define GRID_SIZE 301                /* size of the 2‑D grid (must be odd)       */
#define OFFSET    (GRID_SIZE/2)      /* offset to map coordinates to array index*/

static bool initialized = false;

/* storage for the sequence */
static seq_t  seq_arr[257];          /* a(1) .. a(256)                          */
static bool   visited[MAX_NUM+1];    /* visited flags for numbers 0..MAX_NUM    */
static int16_t x_coord[MAX_NUM+1];   /* x coordinate for each number           */
static int16_t y_coord[MAX_NUM+1];   /* y coordinate for each number           */
static uint32_t grid[GRID_SIZE][GRID_SIZE]; /* mapping from coordinate to number   */
static uint32_t gpf_arr[MAX_NUM+1];   /* greatest prime factor for each number   */

/* one‑time initialisation */
static void init_seq(void)
{
    if (initialized) return;

    /* 1. build the square spiral up to MAX_NUM */
    for (int i = 0; i < GRID_SIZE; ++i)
        for (int j = 0; j < GRID_SIZE; ++j)
            grid[i][j] = 0;

    int x = OFFSET, y = OFFSET;
    uint32_t num = 1;
    grid[x][y] = num;
    x_coord[num] = x;
    y_coord[num] = y;

    int step = 1;
    int dir  = 0;                       /* 0=right, 1=up, 2=left, 3=down       */
    const int dirs[4][2] = {{1,0},{0,-1},{-1,0},{0,1}};

    while (num < MAX_NUM) {
        for (int pair = 0; pair < 2 && num < MAX_NUM; ++pair) {
            for (int s = 0; s < step && num < MAX_NUM; ++s) {
                x += dirs[dir][0];
                y += dirs[dir][1];
                ++num;
                grid[x][y] = num;
                x_coord[num] = x;
                y_coord[num] = y;
            }
            dir = (dir + 1) & 3;
        }
        ++step;
    }

    /* 2. compute greatest prime factors up to MAX_NUM */
    for (uint32_t i = 2; i <= MAX_NUM; ++i) {
        if (gpf_arr[i] == 0) {          /* i is prime */
            for (uint32_t m = i; m <= MAX_NUM; m += i)
                gpf_arr[m] = i;
        }
    }

    /* 3. simulate the king's walk */
    int curr_num = 1;
    visited[1] = true;
    seq_arr[1] = 1;

    for (int i = 2; i <= 256; ++i) {
        int cx = x_coord[curr_num];
        int cy = y_coord[curr_num];
        uint32_t best_num = 0, best_gpf = 0;

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                int nx = cx + dx, ny = cy + dy;
                if (nx < 0 || nx >= GRID_SIZE || ny < 0 || ny >= GRID_SIZE) continue;

                uint32_t nnum = grid[nx][ny];
                if (nnum == 0) continue;
                if (visited[nnum]) continue;

                uint32_t gpf = gpf_arr[nnum];
                if (gpf > best_gpf || (gpf == best_gpf && (best_num == 0 || nnum < best_num))) {
                    best_gpf = gpf;
                    best_num = nnum;
                }
            }
        }

        if (best_num == 0) {            /* no move possible – should not happen before 256 */
            seq_arr[i] = 0;
            break;
        }

        curr_num = best_num;
        visited[curr_num] = true;
        seq_arr[i] = curr_num;
    }

    initialized = true;
}

/* required external function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init_seq();
    if (n <= 256) return seq_arr[(idx_t)n];
    return 0;
}
