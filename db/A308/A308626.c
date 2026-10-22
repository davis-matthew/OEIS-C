/*
 * A308626  Van Eck sequence on a square spiral on a 2-D grid.
 * https://oeis.org/A308626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Van Eck sequence on a square spiral board (implementation
 *            of OEIS A308626)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state */
static seq_t *aseq = NULL;   /* sequence values, 1‑based */
static int   *ax   = NULL;   /* x‑coordinate of each cell, 1‑based */
static int   *ay   = NULL;   /* y‑coordinate of each cell, 1‑based */
static size_t cap  = 0;      /* current allocated capacity (≥ max_n+1) */
static idx_t  max_n = 0;     /* greatest index that has been generated */

/* spiral traversal state */
static int cur_x = 0, cur_y = 0;
static int dir   = 0;   /* 0:right,1:up,2:left,3:down */
static int len   = 1;   /* step length for current direction */
static int rem   = 1;   /* remaining steps in the current direction */
static int dircnt= 0;   /* number of direction changes */

/* Ensure capacity for at least 'need' indices (1‑based). */
static void ensure_capacity(size_t need)
{
    if (need <= cap) return;
    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    aseq = (seq_t *)realloc(aseq, new_cap * sizeof(seq_t));
    ax   = (int   *)realloc(ax,   new_cap * sizeof(int));
    ay   = (int   *)realloc(ay,   new_cap * sizeof(int));
    cap  = new_cap;
}

/* Initialise static data on first call. */
static void init_once(void)
{
    if (max_n) return;
    ensure_capacity(16);          /* generous initial capacity */
    aseq[1] = 0;
    ax[1]   = 0;
    ay[1]   = 0;
    max_n   = 1;
    cur_x   = 0;
    cur_y   = 0;
    dir     = 0;
    len     = 1;
    rem     = 1;
    dircnt  = 0;
}

/* Extend the computation up to index n (inclusive). */
static void extend_to(idx_t n)
{
    static const int DX[4] = {1, 0, -1, 0};
    static const int DY[4] = {0, 1, 0, -1};

    for (; max_n < n; ++max_n) {
        /* Ensure we have space for the next index */
        ensure_capacity(max_n + 2);

        /* Compute a[max_n+1] from the value at max_n */
        const seq_t val = aseq[max_n];
        bool found = false;
        seq_t best = 0;

        for (idx_t j = 1; j < max_n; ++j) {          /* previous indices */
            if (aseq[j] == val) {
                int dx = ax[max_n] - ax[j];
                if (dx < 0) dx = -dx;
                int dy = ay[max_n] - ay[j];
                if (dy < 0) dy = -dy;
                seq_t d = (seq_t)dx + (seq_t)dy;
                if (!found || d < best) { best = d; found = true; }
            }
        }
        aseq[max_n + 1] = found ? best : 0;

        /* Advance to the next cell in the square spiral */
        cur_x += DX[dir];
        cur_y += DY[dir];
        --rem;
        if (rem == 0) {
            dir = (dir + 1) & 3;
            ++dircnt;
            if ((dircnt & 1) == 0) ++len;
            rem = len;
        }
        ax[max_n + 1] = cur_x;
        ay[max_n + 1] = cur_y;
    }
}

/* Public API */
seq_t a(idx_t n)
{
    init_once();
    if (n > max_n) extend_to(n);
    return aseq[n];
}
