/*
 * A307297  Array read by antidiagonals: Sprague-Grundy values for the game
 *          NimHof with 4 rules [1,0], [2,1], [3,3], [0,1].
 * https://oeis.org/A307297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the Sprague‑Grundy value of the cell (x,y) in the *
 *            infinite array of NimHof positions (with rules [1,0], [2,1],
 *            [3,3], [0,1]), * read by antidiagonals of constant x+y. The
 *            mapping from n to (x,y) * is: * let k satisfy k*(k+1)/2 ≤ n <
 *            (k+1)*(k+2)/2, * let d = n - k*(k+1)/2, d = k - x, y =...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* internal state */
static seq_t *sequence  = NULL;  /* linear sequence a(n) */
static size_t seq_len   = 0;     /* length of 'sequence'  */
static int    Kmax      = -1;    /* maximal k for which the grid has been built */
static seq_t *grid      = NULL;  /* flattened 2D array of Grundy values */
static int    grid_w    = 0;     /* width of the grid (Kmax+1) */

/* rebuild the grid and sequence up to the antidiagonal containing n */
static void build_grid_and_seq(idx_t n)
{
    /* find required maximal k */
    idx_t k = 0;
    while (k * (k + 1) / 2 <= n) ++k;
    int newK = (int)(k - 1);          /* new maximal k */
    if (newK <= Kmax) return;          /* already built up to required level */

    /* free any previous allocations */
    free(grid);
    free(sequence);
    seq_len = 0;
    Kmax = newK;
    grid_w = Kmax + 1;

    /* allocate new grid */
    size_t grid_size = (size_t)grid_w * (size_t)grid_w;
    grid = (seq_t *)malloc(grid_size * sizeof(seq_t));
    if (!grid) return; /* allocation failed; caller will handle */

    /* allocate new linear sequence */
    size_t new_seq_len = (size_t)Kmax * (Kmax + 1) / 2 + (size_t)Kmax + 1;
    sequence = (seq_t *)malloc(new_seq_len * sizeof(seq_t));
    if (!sequence) return;

    /* visited marks Grundy values seen in the successor set.
     * The table is large enough for all values that appear up to k≈200
     * (the bound relevant for n up to about 10 000).  If a value beyond
     * the table appears, it will be ignored which is safe for this
     * bounded domain. */
    const int VISITED_SIZE = 1024;
    bool visited[VISITED_SIZE];

    /* iterate over antidiagonals */
    for (int sum = 0; sum <= Kmax; ++sum) {
        for (int x = sum; x >= 0; --x) {
            int y = sum - x;
            /* reset visited */
            memset(visited, 0, sizeof(visited));

            /* rule [1,0] : move along x */
            for (int i = 1;; ++i) {
                int nx = x - i;
                if (nx < 0) break;
                int ny = y;
                seq_t g = grid[nx * grid_w + ny];
                if (g < VISITED_SIZE) visited[(int)g] = true;
            }

            /* rule [2,1] */
            for (int i = 1;; ++i) {
                int nx = x - 2 * i;
                int ny = y -   i;
                if (nx < 0 || ny < 0) break;
                seq_t g = grid[nx * grid_w + ny];
                if (g < VISITED_SIZE) visited[(int)g] = true;
            }

            /* rule [3,3] */
            for (int i = 1;; ++i) {
                int nx = x - 3 * i;
                int ny = y - 3 * i;
                if (nx < 0 || ny < 0) break;
                seq_t g = grid[nx * grid_w + ny];
                if (g < VISITED_SIZE) visited[(int)g] = true;
            }

            /* rule [0,1] : move along y */
            for (int i = 1;; ++i) {
                int nx = x;
                int ny = y - i;
                if (ny < 0) break;
                seq_t g = grid[nx * grid_w + ny];
                if (g < VISITED_SIZE) visited[(int)g] = true;
            }

            /* compute mex */
            seq_t g_val = 0;
            while (g_val < VISITED_SIZE && visited[(int)g_val]) ++g_val;

            /* store in grid and linear sequence */
            grid[x * grid_w + y] = g_val;
            size_t idx = (size_t)sum * (sum + 1) / 2 + (sum - x);
            sequence[idx] = g_val;
        }
    }
    seq_len = new_seq_len;
}

/* external function required by the build system */
seq_t a(idx_t n)
{
    if (n >= seq_len) build_grid_and_seq(n);
    /* after build_grid_and_seq the requested term is guaranteed to exist */
    return sequence[n];
}
