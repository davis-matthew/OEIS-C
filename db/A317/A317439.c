/*
 * A317439  Numbers missing from A317437.
 * https://oeis.org/A317439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - 1 for n <= 11360; a(n) = n + 11360 for n > 11360
 * Range:     u8 n <= 128, u16 n <= 50966, u32 n <= 245051, u64 n <= 245051,
 *            u128 n <= 245051, big any n
 * Validated: standard, pass, 81 terms through n = 9408 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A317439: the numbers missing from A317437, the squares a (1,5)-leaper visits
 * on the square spiral (labels from 0, first step east), moving each time to
 * the unvisited square with the lowest label. The walk is simulated once: it
 * is trapped after 14570 squares, the largest 36666, so from there on every
 * number is missing. (The original returned a closed form that was not this.) */

#include <stdint.h>
#include <stdlib.h>

#define R 200                             /* the board holds |x|, |y| <= R */
#define SIDE (2 * R + 1)

static uint32_t *missing = NULL;          /* missing labels up to the largest visited */
static size_t n_missing = 0;
static uint64_t n_visited = 0, max_visited = 0;

static void build(void)
{
    static int32_t label[SIDE][SIDE];
    static int32_t px[SIDE * SIDE], py[SIDE * SIDE];
    int x = 0, y = 0, d = 0, total = SIDE * SIDE, l = 0;
    static const int dx[4] = {1, 0, -1, 0}, dy[4] = {0, 1, 0, -1};
    for (int i = 0; i < SIDE; ++i)
        for (int j = 0; j < SIDE; ++j) label[i][j] = -1;
    label[R][R] = 0;
    px[0] = py[0] = 0;
    for (int step = 1; l + 1 < total; ++step)
        for (int t = 0; t < 2 && l + 1 < total; ++t, d = (d + 1) & 3)
            for (int s = 0; s < step && l + 1 < total; ++s) {
                x += dx[d];
                y += dy[d];
                if (x < -R || x > R || y < -R || y > R) { l = total; break; }
                label[x + R][y + R] = ++l;
                px[l] = x;
                py[l] = y;
            }
    unsigned char *seen = (unsigned char *)calloc((size_t)total, 1);
    if (!seen) abort();
    static const int mx[8] = {1, 1, -1, -1, 5, 5, -5, -5}, my[8] = {5, -5, 5, -5, 1, -1, 1, -1};
    int cur = 0;
    seen[0] = 1;
    n_visited = 1;
    for (;;) {
        int best = -1;
        for (int m = 0; m < 8; ++m) {
            int qx = px[cur] + mx[m], qy = py[cur] + my[m];
            if (qx < -R || qx > R || qy < -R || qy > R) abort();     /* board too small */
            int q = label[qx + R][qy + R];
            if (q >= 0 && !seen[q] && (best < 0 || q < best)) best = q;
        }
        if (best < 0) break;
        seen[best] = 1;
        ++n_visited;
        if ((uint64_t)best > max_visited) max_visited = (uint64_t)best;
        cur = best;
    }
    missing = (uint32_t *)malloc((size_t)(max_visited + 1) * sizeof *missing);
    if (!missing) abort();
    for (uint64_t m = 0; m <= max_visited; ++m)
        if (!seen[m]) missing[n_missing++] = (uint32_t)m;
    free(seen);
}

seq_t a(idx_t n)
{
    if (!missing) build();
    if (n < 1) return (seq_t)0;
    if ((size_t)n <= n_missing) return (seq_t)missing[n - 1];
    /* past the largest visited square every label m is missing, and it is the
       (m + 1 - n_visited)-th */
    return (seq_t)((uint64_t)n + n_visited - 1);
}
