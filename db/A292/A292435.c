/*
 * A292435  Array T read by antidiagonals: T(m,n) = number of lattice walks of
 *          minimal length from (0,0) to (m,n) using steps in directions from
 *          {(1,0), (0,1), (3,0), (2,1), (1,2), (0,3)}.
 * https://oeis.org/A292435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, walk
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A292435 (Array T read by antidiagonals) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Step vectors: (dx,dy) */
static const idx_t step_dx[6] = {1, 0, 3, 2, 1, 0};
static const idx_t step_dy[6] = {0, 1, 0, 1, 2, 3};

/* Triangular arrays storing T(m,n) and minimal step counts */
static seq_t **cnt   = NULL;   /* counts of optimal walks */
static uint16_t **steps = NULL; /* minimal step numbers */

/* Highest diagonal already computed (decremental indices start at 0) */
static idx_t max_d = (idx_t)-1;
static bool initialized = false;

/* Allocate and compute up to diagonal d (inclusive) */
static void compute_up_to(idx_t d)
{
    if (!initialized) {
        cnt   = (seq_t**)malloc(sizeof(seq_t*));
        steps = (uint16_t**)malloc(sizeof(uint16_t*));
        cnt[0]   = (seq_t*)calloc(1, sizeof(seq_t));
        steps[0] = (uint16_t*)calloc(1, sizeof(uint16_t));
        cnt[0][0]   = (seq_t)1;
        steps[0][0] = 0;
        max_d = 0;
        initialized = true;
    }
    if (d <= max_d) return;

    /* Grow the array pointers */
    cnt   = (seq_t**)realloc(cnt,   sizeof(seq_t*) * (d + 1));
    steps = (uint16_t**)realloc(steps,sizeof(uint16_t*) * (d + 1));
    for (idx_t s = max_d + 1; s <= d; ++s) {
        size_t len = (size_t)(s + 1);
        cnt[s]   = (seq_t*)calloc(len, sizeof(seq_t));
        steps[s] = (uint16_t*)calloc(len, sizeof(uint16_t));
    }

    /* Dynamic programming for new diagonals */
    for (idx_t s = max_d + 1; s <= d; ++s) {
        for (idx_t m = 0; m <= s; ++m) {
            idx_t n = s - m;              /* n = s - m */
            uint16_t best = 0xFFFF;
            seq_t tcnt = 0;
            for (int k = 0; k < 6; ++k) {
                idx_t dx = step_dx[k];
                idx_t dy = step_dy[k];
                if (m < dx || n < dy) continue;
                idx_t prev_s = s - (dx + dy);
                idx_t prev_m = m - dx;
                uint16_t cand = steps[prev_s][prev_m] + 1;
                if (cand < best) {
                    best = cand;
                    tcnt = cnt[prev_s][prev_m];
                } else if (cand == best) {
                    tcnt += cnt[prev_s][prev_m];
                }
            }
            steps[s][m] = best;
            cnt[s][m]   = tcnt;
        }
    }
    max_d = d;
}

/* Map OEIS index n to diagonal d and coordinates (m,n_coord) */
static void index_to_coords(idx_t n, idx_t *d, idx_t *m, idx_t *n_coord)
{
    /* Solve d(d+1)/2 <= n < (d+1)(d+2)/2 */
    long double root = sqrt((long double)(8 * (long double)n + 1));
    idx_t d_guess = (idx_t)((root - 1.0L) / 2.0L);
    while (d_guess * (d_guess + 1) / 2 > n) d_guess--;
    while ((d_guess + 1) * (d_guess + 2) / 2 <= n) d_guess++;
    *d = d_guess;
    idx_t first = *d * (*d + 1) / 2;
    idx_t offset = n - first;
    *m = offset;
    *n_coord = *d - offset;
}

seq_t a(idx_t n)
{
    idx_t d, m, n_coord;
    index_to_coords(n, &d, &m, &n_coord);
    compute_up_to(d);
    return cnt[d][m];
}
