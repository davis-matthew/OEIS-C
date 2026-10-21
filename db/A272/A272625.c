/*
 * A272625  Array read by antidiagonals: T(n,k) = number of ways to choose 3
 *          distinct points from an n X k rectangular grid so that they form
 *          an acute isosceles triangle of nonzero area.
 * https://oeis.org/A272625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 575, u32 n <= 3003, u64 n <= 3003,
 *            u128 n <= 3003, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A272625: a(n) = T(i,j) where (i,j) is the point at position n when the 2‑D array T(n,k) is read by antidiagonals. 
   T(n,k) is the number of ways to choose 3 distinct points from an n×k rectangular lattice so that they form an acute isosceles triangle of nonzero area. */

/* Helper structures and functions for computing T(n,k). */
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int dx;
    int dy;
    int d;            /* squared length dx^2+dy^2 */
} Vector;

static int cmp_vector(const void *a, const void *b)
{
    int da = ((const Vector *)a)->d;
    int db = ((const Vector *)b)->d;
    return (da > db) - (da < db);
}

/* Compute T(n,k) for a given rectangle size (n,k). */
static seq_t compute_T(idx_t n_i, idx_t k_i)
{
    if (n_i < 2 || k_i < 2) return 0;

    int n = (int)n_i;
    int k = (int)k_i;

    /* Create list of all non‑zero vectors in the grid. */
    int totalVec = (2 * k - 1) * (2 * n - 1) - 1;
    Vector *vecs = (Vector *)malloc(sizeof(Vector) * totalVec);
    if (!vecs) return 0;
    int idx = 0;
    for (int dx = -(k - 1); dx <= (k - 1); ++dx) {
        for (int dy = -(n - 1); dy <= (n - 1); ++dy) {
            if (dx == 0 && dy == 0) continue;
            vecs[idx].dx = dx;
            vecs[idx].dy = dy;
            vecs[idx].d  = dx * dx + dy * dy;
            ++idx;
        }
    }

    /* Sort vectors by squared length so equal lengths are consecutive. */
    qsort(vecs, idx, sizeof(Vector), cmp_vector);

    seq_t total = 0;
    /* Process each group of vectors that share the same squared length. */
    int s = 0;
    while (s < idx) {
        int e = s + 1;
        int current_d = vecs[s].d;
        while (e < idx && vecs[e].d == current_d) ++e;
        int groupSize = e - s;

        for (int i = s; i < e; ++i) {
            int dx1 = vecs[i].dx;
            int dy1 = vecs[i].dy;
            for (int j = i + 1; j < e; ++j) {
                int dx2 = vecs[j].dx;
                int dy2 = vecs[j].dy;

                int dot = dx1 * dx2 + dy1 * dy2;
                if (dot <= 0) continue;          /* not an acute apex angle */

                /* Count admissible apex positions in x‑direction. */
                int a_x = (dx1 > dx2) ? dx1 : dx2;
                if (a_x < 0) a_x = 0;
                int b_x = (dx1 < dx2) ? dx1 : dx2;
                if (b_x > 0) b_x = 0;
                int cntX = k + b_x - a_x;
                if (cntX <= 0) continue;

                /* Count admissible apex positions in y‑direction. */
                int a_y = (dy1 > dy2) ? dy1 : dy2;
                if (a_y < 0) a_y = 0;
                int b_y = (dy1 < dy2) ? dy1 : dy2;
                if (b_y > 0) b_y = 0;
                int cntY = n + b_y - a_y;
                if (cntY <= 0) continue;

                total += (seq_t)cntX * (seq_t)cntY;
            }
        }
        s = e;
    }

    free(vecs);
    return total;
}

/* Convert the linear index n of the antidiagonal enumeration to the pair (i,j) in the grid. */
static idx_t diagonal_of_idx(idx_t idx)
{
    /* Find the smallest d with d*(d-1)/2 >= idx. */
    idx_t low = 1, high = 2 * idx;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t cum = mid * (mid - 1) / 2;
        if (cum >= idx) high = mid;
        else low = mid + 1;
    }
    return low;
}

/* Main sequence function as required. */
seq_t a(idx_t idx)
{
    if (idx == 0) return 0;          /* not part of the defined range */

    idx_t d = diagonal_of_idx(idx);
    idx_t cum_prev = (d - 1) * (d - 2) / 2;
    idx_t pos = idx - cum_prev;      /* 1‑based position within this antidiagonal */

    idx_t i = pos;                   /* first dimension of the rectangle */
    idx_t j = d - pos;               /* second dimension */

    return compute_T(i, j);
}
