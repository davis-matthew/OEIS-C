/*
 * A334742  Pascal's spiral: starting with a(1) = 1, proceed in a square
 *          spiral, computing each term as the sum of horizontally and
 *          vertically adjacent prior terms.
 * https://oeis.org/A334742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 1 followed by values obtained by walking in a * square
 *            spiral on an unbounded grid; each new cell contains the sum * of
 *            the horizontally and vertically adjacent cells that are already
 *            * present. a(1)=1. The order of traversal is the usual clockwise
 *            * spiral starting to the right of the centre
 * Range:     u8 n <= 45, u16 n <= 120, u32 n <= 338, u64 n <= 994,
 *            u128 n <= 3005, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/*  Static state (global, but limited to this translation unit).     */
/* ------------------------------------------------------------------ */
static seq_t *vals      = NULL;      /* sequence values, 0‑based   */
static size_t vals_cap  = 0;          /* capacity of vals array    */
static size_t vals_size = 0;          /* number of stored values   */

static seq_t *grid = NULL;           /* 2‑D grid as 1‑D array     */
static size_t side = 0;              /* current grid side length  */
static int64_t offset = 0;           /* offset of origin in grid  */

static int cur_dir = 0;              /* 0=R,1=D,2=L,3=U          */
static int cur_step_len = 1;         /* steps in current direction*/
static int cur_steps_in_dir = 0;     /* steps taken in current dir*/
static int64_t cur_x = 0, cur_y = 0; /* current position         */

static const int dx[4] = { 1, 0, -1, 0 };
static const int dy[4] = { 0, -1, 0, 1 };

/* ------------------------------------------------------------------ */
/*  Helper: grow the values array if needed.                           */
/* ------------------------------------------------------------------ */
static void ensure_vals_capacity(size_t need)
{
    if (need <= vals_cap)
        return;
    size_t new_cap = vals_cap ? vals_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    vals = tmp;
    vals_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Helper: enlarge the grid so that (x,y) fits within it.            */
/* ------------------------------------------------------------------ */
static void enlarge_grid_if_needed(int64_t x, int64_t y)
{
    while (llabs(x) > offset || llabs(y) > offset) {
        size_t old_side   = side;
        int64_t old_off   = offset;
        size_t new_side   = old_side ? old_side * 2 + 1 : 1;
        int64_t new_off   = new_side / 2;

        seq_t *new_grid = (seq_t *)calloc(new_side * new_side, sizeof(seq_t));
        if (!new_grid)
            abort();

        /* Copy old data into new grid. */
        for (int64_t yy = -old_off; yy <= old_off; ++yy) {
            for (int64_t xx = -old_off; xx <= old_off; ++xx) {
                seq_t v = grid[(yy + old_off) * old_side + (xx + old_off)];
                if (v) {
                    size_t nidx = (yy + new_off) * new_side + (xx + new_off);
                    new_grid[nidx] = v;
                }
            }
        }

        free(grid);
        grid  = new_grid;
        side  = new_side;
        offset = new_off;
    }
}

/* ------------------------------------------------------------------ */
/*  Helper: compute the next value in the spiral order.                */
/* ------------------------------------------------------------------ */
static void compute_next(void)
{
    /* Move one step in the current direction. */
    cur_x += dx[cur_dir];
    cur_y += dy[cur_dir];

    /* Ensure the grid can hold the new position. */
    enlarge_grid_if_needed(cur_x, cur_y);

    /* Sum the existing horizontal and vertical neighbours. */
    seq_t sum = 0;
    for (int k = 0; k < 4; ++k) {
        int64_t nx = cur_x + (k == 0 ? -1 : k == 1 ? 1 : 0);
        int64_t ny = cur_y + (k == 2 ? -1 : k == 3 ? 1 : 0);
        if (nx >= -offset && nx <= offset && ny >= -offset && ny <= offset) {
            size_t nidx = (ny + offset) * side + (nx + offset);
            if (grid[nidx])
                sum += grid[nidx];
        }
    }

    /* Store the new value. */
    ensure_vals_capacity(vals_size + 1);
    vals[vals_size++] = sum;

    size_t cur_idx = (cur_y + offset) * side + (cur_x + offset);
    grid[cur_idx] = sum;

    /* Advance spiral state. */
    ++cur_steps_in_dir;
    if (cur_steps_in_dir == cur_step_len) {
        cur_steps_in_dir = 0;
        cur_dir = (cur_dir + 1) & 3;
        if ((cur_dir & 1) == 0)  /* after every two turns */
            ++cur_step_len;
    }
}

/* ------------------------------------------------------------------ */
/*  Public function – compute a(n).                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined, but harmless */

    /* Lazy initialization of the sequence and grid. */
    if (!vals) {
        vals_cap = 16;
        vals = (seq_t *)malloc(vals_cap * sizeof(seq_t));
        if (!vals)
            abort();
        vals_size = 1;
        vals[0] = (seq_t)1;     /* a(1) = 1 */

        side = 1;
        offset = 0;
        grid = (seq_t *)calloc(1, sizeof(seq_t));
        if (!grid)
            abort();
        grid[0] = (seq_t)1;

        cur_x = 0;
        cur_y = 0;
        cur_dir = 0;
        cur_step_len = 1;
        cur_steps_in_dir = 0;
    }

    /* Generate values until we have reached the requested index. */
    while (vals_size < n) {
        compute_next();
    }

    return vals[n - 1];
}
