/*
 * A326410  Minesweeper sequence of positive integers arranged on a square
 *          spiral on a 2D grid.
 * https://oeis.org/A326410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) for A326410 (Minesweeper sequence) Place positive integers
 *            on a square spiral, replace primes by -1, and nonprimes by the
 *            number of primes in the eight adjacent grid cells
 * Range:     i8 n <= 9618, i16 n <= 9618, i32 n <= 9618, i64 n <= 9618,
 *            i128 n <= 9618, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> /* for sqrt used in local sqrt fallback */

static idx_t max_n_cached = 0;
static idx_t *pos_x = NULL;
static idx_t *pos_y = NULL;
static idx_t *grid = NULL;
static bool *is_prime = NULL;
static idx_t grid_offset, grid_size;

/* helper: integer sqrt via built-in if available or fallback */
static inline idx_t int_sqrt(idx_t v) {
#ifdef _MSC_VER
    return (idx_t)floor(sqrt((double)v));
#else
    return (idx_t)floor(sqrt((double)v));
#endif
}

/* Build cache up to nmax */
static void init_cache(idx_t nmax) {
    if (nmax <= max_n_cached) return;

    /* free old memory */
    free(pos_x); free(pos_y); free(grid); free(is_prime);

    /* radius of square that will contain nmax and surrounding neighbors */
    idx_t rad = int_sqrt(nmax);
    if (rad * rad < nmax) rad++;
    rad += 5;                       /* safety margin */
    grid_offset = rad;
    grid_size = 2 * rad + 1;
    idx_t limit = grid_size * grid_size;   /* largest integer in grid */

    /* allocate structures */
    pos_x = (idx_t *)calloc(nmax + 1, sizeof(idx_t));
    pos_y = (idx_t *)calloc(nmax + 1, sizeof(idx_t));
    grid   = (idx_t *)calloc(grid_size * grid_size, sizeof(idx_t));
    is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!pos_x || !pos_y || !grid || !is_prime) return; /* allocation failure */

    /* generate the spiral up to limit */
    int x = 0, y = 0;
    pos_x[1] = 0; pos_y[1] = 0;
    grid[grid_offset * grid_size + grid_offset] = 1;

    idx_t cnt = 1;
    int step = 1;
    int dir = 0; /* 0:right,1:up,2:left,3:down */

    while (cnt < limit) {
        for (int t = 0; t < 2 && cnt < limit; ++t) {
            for (int i = 0; i < step && cnt < limit; ++i) {
                ++cnt;
                switch (dir) {
                    case 0: ++x; break;
                    case 1: ++y; break;
                    case 2: --x; break;
                    case 3: --y; break;
                }
                if (cnt <= nmax) {
                    pos_x[cnt] = (idx_t)x;
                    pos_y[cnt] = (idx_t)y;
                }
                grid[(y + (int)grid_offset) * grid_size + (x + (int)grid_offset)] = cnt;
            }
            dir = (dir + 1) & 3;
        }
        ++step;
    }

    /* sieve primes up to limit */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p) is_prime[j] = false;
        }
    }

    max_n_cached = nmax;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but avoid crash */
    if (n > max_n_cached) init_cache(n);

    if (is_prime[n]) return (seq_t)-1;

    int count = 0;
    static const int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    static const int dy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

    for (int k = 0; k < 8; ++k) {
        int ix = (int)pos_x[n] + dx[k];
        int iy = (int)pos_y[n] + dy[k];
        idx_t gx = (idx_t)(ix + (int)grid_offset);
        idx_t gy = (idx_t)(iy + (int)grid_offset);
        if (gx < grid_size && gy < grid_size) {
            idx_t num = grid[gy * grid_size + gx];
            if (num > 0 && is_prime[num]) ++count;
        }
    }
    return (seq_t)count;
}
