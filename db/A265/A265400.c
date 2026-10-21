/*
 * A265400  a(n) = one-based index to the nearest horizontally or vertically
 *          adjacent inner neighbor in square-grid spirals, or 0 if n is one
 *          of the corner cases A033638.
 * https://oeis.org/A265400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 322, u16 n <= 9230, u32 n <= 9230, u64 n <= 9230,
 *            u128 n <= 9230, big any n
 * Helpers:   is_corner
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A265400: for each n the index of the nearest horizontally or vertically
   adjacent inner neighbour in the counter‑clockwise Ulam spiral, or 0 if n is
   one of the corner indices A033638.  The implementation builds the spiral
   up to n, stores the coordinates, and then checks the four orthogonal
   neighbours.  A corner index is detected by the recurrence
   a_1 = 1, a_k = a_{k-1} + floor(k/2).  */
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>
#include "../utils/is_corner.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (is_corner(n)) return 0;        /* zero for corner cases */

    /* -------- build spiral up to n, storing coordinates -------- */
    idx_t maxCoord = (idx_t)(sqrt((double)n) + 2);   /* safety margin */
    idx_t offset   = maxCoord;
    idx_t width    = offset * 2 + 1;
    size_t mapSize = (size_t)width * (size_t)width;

    int *map   = (int *)calloc(mapSize, sizeof(int));
    int *xarr  = (int *)malloc((size_t)(n + 1) * sizeof(int));
    int *yarr  = (int *)malloc((size_t)(n + 1) * sizeof(int));
    if (!map || !xarr || !yarr) {
        free(map); free(xarr); free(yarr);
        return 0;
    }

    int x = 0, y = 0;
    xarr[1] = 0; yarr[1] = 0;
    map[offset * width + offset] = 1;

    idx_t idx = 2;
    int dir = 0;          /* 0:right, 1:up, 2:left, 3:down */
    int stepLen = 1;
    int turns = 0;

    while (idx <= n) {
        for (int step = 0; step < stepLen && idx <= n; ++step) {
            if (dir == 0) ++x;
            else if (dir == 1) ++y;
            else if (dir == 2) --x;
            else --y;

            xarr[idx] = x;
            yarr[idx] = y;
            map[(x + offset) * width + (y + offset)] = (int)idx;
            ++idx;
        }
        dir = (dir + 1) & 3;
        ++turns;
        if (turns % 2 == 0) ++stepLen;
    }

    int xn = xarr[n];
    int yn = yarr[n];

    /* -------- find nearest inner neighbour -------- */
    int bestIdx = 0;
    int bestDist = INT_MAX;
    int dxy[4][2] = { {1,0}, {0,1}, {-1,0}, {0,-1} };

    for (int d = 0; d < 4; ++d) {
        int nx = xn + dxy[d][0];
        int ny = yn + dxy[d][1];
        int neighIdx = 0;
        int mapIdx = (nx + offset) * width + (ny + offset);
        if (mapIdx >= 0 && mapIdx < (int)mapSize)
            neighIdx = map[mapIdx];
        if (neighIdx > 0 && (idx_t)neighIdx < n) {
            int dist = abs(nx) + abs(ny);   /* Manhattan distance to origin */
            if (dist < bestDist || (dist == bestDist && neighIdx < bestIdx)) {
                bestDist = dist;
                bestIdx = neighIdx;
            }
        }
    }

    free(map);
    free(xarr);
    free(yarr);
    return (seq_t)bestIdx;
}
