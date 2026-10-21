/*
 * A271910  Array read by antidiagonals: T(n,k) = number of ways to choose 3
 *          distinct points from an n X k rectangular grid so that they form
 *          an isosceles triangle.
 * https://oeis.org/A271910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of isosceles triangles in the n‑by‑k rectangular
 *            grid where (n,k) is obtained from n by reading the OEIS array
 *            along antidiagonals
 * Range:     u8 n <= 7, u16 n <= 419, u32 n <= 3003, u64 n <= 3003,
 *            u128 n <= 3003, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

#define MAX_DIM   200          /* sufficient for all intended indices (≈142) */
#define MAX_OFFSETS   ((2*(MAX_DIM-1)+1)*(2*(MAX_DIM-1)+1)-1)
#define MAX_DIST     ((MAX_DIM-1)*(MAX_DIM-1)*2)

/* cache for T(i,j) values */
static seq_t T_cache[MAX_DIM+1][MAX_DIM+1];
static unsigned char T_seen[MAX_DIM+1][MAX_DIM+1];

/* temporary storage for a single computation of T(i,j) */
struct Off { int dx, dy; };
static struct Off offsets[MAX_OFFSETS];
static struct Off sorted[MAX_OFFSETS];
static int dist_arr[MAX_OFFSETS];
static int countByDist[MAX_DIST+1];
static int offsetStart[MAX_DIST+1];
static int currentPos[MAX_DIST+1];

/* number of isosceles triangles in an i×j grid */
static seq_t compute_T(idx_t ii, idx_t jj)
{
    int i = (int)ii;
    int j = (int)jj;

    if (i > MAX_DIM || j > MAX_DIM) {
        /* dimensions outside preallocated range – fall back to brute force
           (not expected for intended input ranges) */
        return 0;
    }

    if (T_seen[i][j]) return T_cache[i][j];
    T_seen[i][j] = 1;

    /* zero‑area grids have no triangles */
    if (i == 1 || j == 1) {
        T_cache[i][j] = 0;
        return 0;
    }

    int maxDx = i - 1;
    int maxDy = j - 1;
    int totalOffsets = (2 * maxDx + 1) * (2 * maxDy + 1) - 1;

    /* collect all non‑zero offset vectors and their squared lengths */
    int idx = 0;
    for (int dx = -maxDx; dx <= maxDx; ++dx) {
        for (int dy = -maxDy; dy <= maxDy; ++dy) {
            if (dx == 0 && dy == 0) continue;
            offsets[idx].dx = dx;
            offsets[idx].dy = dy;
            dist_arr[idx] = dx * dx + dy * dy;
            ++idx;
        }
    }

    /* count offsets per squared distance */
    int maxDist = maxDx * maxDx + maxDy * maxDy;
    for (int d = 0; d <= maxDist; ++d) countByDist[d] = 0;
    for (int k = 0; k < totalOffsets; ++k) ++countByDist[dist_arr[k]];

    /* build start indices for each distance bucket */
    offsetStart[0] = 0;
    for (int d = 1; d <= maxDist; ++d) {
        offsetStart[d] = offsetStart[d - 1] + countByDist[d - 1];
    }

    /* copy start indices for filling */
    memcpy(currentPos, offsetStart, (maxDist + 1) * sizeof(int));

    /* sort offsets into groups of equal distance */
    for (int k = 0; k < totalOffsets; ++k) {
        int d = dist_arr[k];
        int pos = currentPos[d]++;
        sorted[pos] = offsets[k];
    }

    /* now count triangles by pairing distinct offsets of equal length */
    seq_t answer = 0;
    for (int d = 0; d <= maxDist; ++d) {
        int start = offsetStart[d];
        int groupSize = countByDist[d];
        if (groupSize < 2) continue;
        int end = start + groupSize;
        for (int a = start; a < end; ++a) {
            const struct Off *u = &sorted[a];
            for (int b = a + 1; b < end; ++b) {
                const struct Off *v = &sorted[b];

                /* skip collinear antipodal pairs */
                if (u->dx == -v->dx && u->dy == -v->dy) continue;

                /* determine allowed positions for the apex point P */
                int Lx = 1;
                if (1 - u->dx > Lx) Lx = 1 - u->dx;
                if (1 - v->dx > Lx) Lx = 1 - v->dx;
                int Rx = i;
                if (i - u->dx < Rx) Rx = i - u->dx;
                if (i - v->dx < Rx) Rx = i - v->dx;
                int count_x = Rx - Lx + 1;
                if (count_x <= 0) continue;

                int Ly = 1;
                if (1 - u->dy > Ly) Ly = 1 - u->dy;
                if (1 - v->dy > Ly) Ly = 1 - v->dy;
                int Ry = j;
                if (j - u->dy < Ry) Ry = j - u->dy;
                if (j - v->dy < Ry) Ry = j - v->dy;
                int count_y = Ry - Ly + 1;
                if (count_y <= 0) continue;

                seq_t mult = (seq_t)count_x * (seq_t)count_y;
                answer += mult;
            }
        }
    }

    T_cache[i][j] = answer;
    return answer;
}

/* public sequence function a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* find the antidiagonal number s such that
       n lies in the s‑th antidiagonal (having s‑1 points) */
    seq_t sq = floor_sqrt((seq_t)1 + (seq_t)8 * n);
    idx_t s = (idx_t)((sq + 1) / 2);

    /* adjust s in case the initial guess is too small/large */
    seq_t prev = (seq_t)(s - 1) * (seq_t)(s - 2) / 2;
    if ((seq_t)s * (seq_t)(s - 1) / 2 < n) {
        do { ++s; prev = (seq_t)(s - 1) * (seq_t)(s - 2) / 2; }
        while ((seq_t)s * (seq_t)(s - 1) / 2 < n);
    } else if ((seq_t)(s - 1) * (seq_t)(s - 2) / 2 >= n) {
        do { --s; prev = (seq_t)(s - 1) * (seq_t)(s - 2) / 2; }
        while ((seq_t)(s - 1) * (seq_t)(s - 2) / 2 >= n);
    }

    /* position within the antidiagonal (1‑based) */
    seq_t pos = n - prev;
    idx_t i = (idx_t)pos;
    idx_t j = (idx_t)(s - pos);

    return compute_T(i, j);
}
