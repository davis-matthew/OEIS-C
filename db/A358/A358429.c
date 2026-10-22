/*
 * A358429  Construct a square spiral: a(n) is the sum of all adjacent terms
 *          a(k) in the spiral for k < n; a(1) = 0, a(2) = 1.
 * https://oeis.org/A358429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 1; for n ≥ 3, a(n) = sum of all a(k) for k < n
 *            whose positions are the 8 neighboring cells of the position of
 *            a(n) in the square spiral
 * Range:     u8 n <= 21, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define GRID_SIZE 405               /* sufficient for n ≤ 10000      */
#define GRID_OFFSET (GRID_SIZE/2)    /* center of the grid             */

static seq_t *seq_vals   = NULL;   /* stored values a[1..]          */
static int   *coord_x    = NULL;   /* x‑coordinate of each a(n)     */
static int   *coord_y    = NULL;   /* y‑coordinate of each a(n)     */
static size_t seq_cap    = 0;      /* capacity of the above arrays  */

static seq_t *grid_vals  = NULL;   /* flattened GRID_SIZE*GRID_SIZE grid */
static uint8_t *grid_flag = NULL;  /* bitmap of occupied cells       */

static idx_t  gen_index       = 1;  /* last computed index           */
static int    gen_x           = 0;  /* current x‑coordinate of generator */
static int    gen_y           = 0;  /* current y‑coordinate of generator */
static int    gen_dir         = 0;  /* 0:right, 1:up, 2:left, 3:down      */
static int    gen_step        = 1;  /* current step length              */
static int    gen_steps_this  = 0;  /* steps taken in current direction  */
static bool   initialized     = false;   /* flag for first call             */

static inline size_t grid_index(int x, int y) {
    int gx = x + GRID_OFFSET;
    int gy = y + GRID_OFFSET;
    return (size_t)gy * GRID_SIZE + gx;
}

seq_t a(idx_t n) {
    /* Initialise data structures on first call. */
    if (!initialized) {
        seq_cap = 64;
        seq_vals   = (seq_t*)malloc((seq_cap + 1) * sizeof(seq_t));
        coord_x    = (int*)malloc((seq_cap + 1) * sizeof(int));
        coord_y    = (int*)malloc((seq_cap + 1) * sizeof(int));

        grid_vals  = (seq_t*)malloc(GRID_SIZE * GRID_SIZE * sizeof(seq_t));
        grid_flag  = (uint8_t*)calloc(GRID_SIZE * GRID_SIZE, sizeof(uint8_t));

        seq_vals[1] = (seq_t)0;
        coord_x[1]  = 0;
        coord_y[1]  = 0;
        size_t gidx = grid_index(0, 0);
        grid_flag[gidx] = 1;
        grid_vals[gidx] = seq_vals[1];

        initialized = true;
    }

    /* Return previously computed value if possible. */
    if (n <= gen_index) {
        return seq_vals[n];
    }

    /* Enlarge stored arrays if required. */
    if (n > (idx_t)seq_cap) {
        size_t new_cap = seq_cap * 2;
        if (new_cap < (size_t)n + 1) {
            new_cap = (size_t)n + 1;
        }
        seq_vals   = (seq_t*)realloc(seq_vals,   (new_cap + 1) * sizeof(seq_t));
        coord_x    = (int*)realloc(coord_x,    (new_cap + 1) * sizeof(int));
        coord_y    = (int*)realloc(coord_y,    (new_cap + 1) * sizeof(int));
        seq_cap = new_cap;
    }

    /* Generate terms up to n. */
    for (idx_t idx = gen_index + 1; idx <= n; ++idx) {
        /* Move generator one step. */
        switch (gen_dir) {
            case 0: ++gen_x; break;          /* right  */
            case 1: ++gen_y; break;          /* up     */
            case 2: --gen_x; break;          /* left   */
            case 3: --gen_y; break;          /* down   */
        }

        seq_t value;
        if (idx == 2) {
            value = (seq_t)1;
        } else {
            /* Sum all adjacent previously placed values. */
            seq_t sum = 0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = gen_x + dx;
                    int ny = gen_y + dy;
                    size_t gidx = grid_index(nx, ny);
                    if (grid_flag[gidx]) {
                        sum += grid_vals[gidx];
                    }
                }
            }
            value = sum;
        }

        seq_vals[idx] = value;
        coord_x[idx]  = gen_x;
        coord_y[idx]  = gen_y;

        size_t gidx = grid_index(gen_x, gen_y);
        grid_flag[gidx] = 1;
        grid_vals[gidx] = value;

        /* Update step counters. */
        ++gen_steps_this;
        if (gen_steps_this == gen_step) {
            gen_steps_this = 0;
            gen_dir = (gen_dir + 1) % 4;
            if (gen_dir == 2 || gen_dir == 0) { /* two sides completed */
                ++gen_step;
            }
        }
    }

    gen_index = n;
    return seq_vals[n];
}
