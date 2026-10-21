/*
 * A278180  Square spiral in which each new term is the sum of its two largest
 *          neighbors.
 * https://oeis.org/A278180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 58, u32 n <= 183, u64 n <= 601,
 *            u128 n <= 1991, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A278180:
   Square spiral where each new term is the sum of its two largest neighbors. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <new>
#endif

/* Grid constants: radius of the square surrounding the spiral. */
static const int  OFFSET      = 256;
static const int  GRID_SIZE   = OFFSET * 2 + 1;

/* Memoised data structures. */
static seq_t *grid          = NULL;   /* 2‑D grid flattened to 1‑D */
static seq_t *seq           = NULL;   /* sequence values, zero‑based */
static idx_t  seq_len       = 0;      /* number of terms stored */
static idx_t  seq_cap       = 0;      /* capacity of seq array */

/* Current position in the spiral. */
static int    cur_x        = 0;
static int    cur_y        = 0;
static int    dir_idx      = 0;   /* 0:right,1:up,2:left,3:down */
static int    step_len     = 1;
static int    step_done    = 0;
static int    pair_done    = 0;

/* Direction vectors. */
static const int dx[4] = { 1, 0, -1, 0 };
static const int dy[4] = { 0, 1, 0, -1 };

/* Offsets to all 8 neighbours. */
static const int nbr_dx[8] = { -1, -1, -1,  0, 0,  1,  1,  1 };
static const int nbr_dy[8] = { -1,  0,  1, -1, 1, -1,  0,  1 };

/* Allocate and default‑construct grid if not yet allocated. */
static void init_grid(void)
{
    if (!grid) {
        size_t total = (size_t)GRID_SIZE * GRID_SIZE;
        grid = (seq_t*)malloc(total * sizeof(seq_t));
        if (!grid) return;               /* out of memory – caller will fail */
#ifdef __cplusplus
        for (size_t i = 0; i < total; ++i)
            new (&grid[i]) seq_t();
#else
        memset(grid, 0, total * sizeof(seq_t));
#endif
    }
}

/* Ensure seq array can hold at least 'need' elements. */
static void ensure_seq_capacity(idx_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 128;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_seq = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_seq) return;                /* out of memory – caller will fail */
#ifdef __cplusplus
    for (size_t i = 0; i < new_cap; ++i)
        new (&new_seq[i]) seq_t();
#else
    memset(new_seq, 0, new_cap * sizeof(seq_t));
#endif
    if (seq) {
        memcpy(new_seq, seq, (size_t)seq_len * sizeof(seq_t));
        free(seq);
    }
    seq = new_seq;
    seq_cap = new_cap;
}

/* Linear index of (x,y) within the flattened grid. */
static inline seq_t *grid_at(int x, int y)
{
    return &grid[(y + OFFSET) * GRID_SIZE + (x + OFFSET)];
}

/* Generate the next term of the sequence. */
static seq_t compute_next(void)
{
    /* Advance one step. */
    cur_x += dx[dir_idx];
    cur_y += dy[dir_idx];
    ++step_done;

    /* Update direction and step length. */
    if (step_done == step_len) {
        step_done = 0;
        dir_idx = (dir_idx + 1) & 3;
        ++pair_done;
        if (pair_done == 2) {
            pair_done = 0;
            ++step_len;
        }
    }

    /* Scan neighbours to find two largest values. */
    seq_t max1 = 0, max2 = 0;
    for (int k = 0; k < 8; ++k) {
        seq_t nval = *grid_at(cur_x + nbr_dx[k], cur_y + nbr_dy[k]);
        if (nval > max1) {
            max2 = max1;
            max1 = nval;
        } else if (nval > max2) {
            max2 = nval;
        }
    }

    seq_t result = max1 + max2;
    *grid_at(cur_x, cur_y) = result;

    seq[seq_len] = result;
    ++seq_len;
    return result;
}

/* Public interface requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* offset is 1; guard against illegal input */
    init_grid();
    ensure_seq_capacity(n);

    /* Initialise the first term if nothing computed yet. */
    if (seq_len == 0) {
        *grid_at(0, 0) = 1;
        seq[0] = 1;
        seq_len = 1;
        cur_x = 0;
        cur_y = 0;
        dir_idx = 0;
        step_len = 1;
        step_done = 0;
        pair_done = 0;
    }

    /* Generate terms until we reach the requested index. */
    while (seq_len < n) {
        compute_next();
    }

    return seq[n - 1];
}
