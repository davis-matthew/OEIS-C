/*
 * A316884  Squares visited by moving diagonally one square on a spirally
 *          numbered board and moving to the lowest available unvisited square
 *          at each step.
 * https://oeis.org/A316884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   num_from_coord
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function simulates the walk described in A316884:
   start at 1; at each step move diagonally to one of the four
   neighboring cells, choosing the unvisited cell with the lowest
   spiral number.  a(n) is the spiral number of the square visited
   at step n (a(1)=1). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_from_coord.h"

/* integer square root for 64‑bit unsigned values */
static idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) lo = mid + 1;
        else        hi = mid - 1;
    }
    return hi;
}

/* convert spiral number to (x,y) coordinates (for 64‑bit values) */
static void coord_from_num(uint64_t num, int64_t *x, int64_t *y) {
    if (num == 1) {
        *x = 0; *y = 0; return;
    }
    /* determine ring r */
    uint64_t m = isqrt_u64(num);
    uint64_t r;
    if (m * m == num) {
        r = (m & 1) ? (m - 1) / 2 : m / 2;
    } else {
        r = (m + 1) / 2;
    }
    uint64_t side = 2 * r;
    uint64_t max_num = (2 * r + 1);
    max_num = max_num * max_num;
    uint64_t t = max_num - num;
    int64_t rr = (int64_t)r;
    if (t < side) {               /* bottom side, moving left */
        *x = rr - (int64_t)t;
        *y = -rr;
    } else if (t < 2 * side) {    /* left side, moving up */
        t -= side;
        *x = -rr;
        *y = -rr + (int64_t)t;
    } else if (t < 3 * side) {    /* top side, moving right */
        t -= 2 * side;
        *x = -rr + (int64_t)t;
        *y = rr;
    } else {                      /* right side, moving down */
        t -= 3 * side;
        *x = rr;
        *y = rr - (int64_t)t;
    }
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* undefined below offset */
    if (n == 1) return 1;

    /* visited numbers stored as 64‑bit array for efficiency */
    uint64_t *visited = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    if (!visited) return 0;               /* allocation failure */
    size_t seen = 1;
    visited[0] = 1;

    uint64_t cur = 1;
    /* four diagonal direction vectors */
    const int dx[4] = { 1,  1, -1, -1 };
    const int dy[4] = { 1, -1,  1, -1 };

    for (idx_t step = 2; step <= n; ++step) {
        int64_t cx, cy;
        coord_from_num(cur, &cx, &cy);
        uint64_t best = 0;
        bool found = false;
        for (int k = 0; k < 4; ++k) {
            int64_t nx = cx + dx[k];
            int64_t ny = cy + dy[k];
            uint64_t cand = num_from_coord(nx, ny);
            /* check if cand already visited */
            bool used = false;
            for (size_t j = 0; j < seen; ++j) {
                if (visited[j] == cand) {
                    used = true; break;
                }
            }
            if (used) continue;
            if (!found || cand < best) {
                best = cand;
                found = true;
            }
        }
        cur = best;
        visited[seen++] = cur;
    }

    free(visited);
    return (seq_t)cur;
}
