/*
 * A292276  a(n) = number of vertices of the convex hull of the set of points
 *          of norm <= n^2 in square lattice.
 * https://oeis.org/A292276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A292276: the number of vertices of the convex hull of all
 * integer lattice points (x,y) with x^2 + y^2 <= n^2.
 *
 * The algorithm constructs the upper hull of the point set
 * { (x, floor(sqrt(n^2 - x^2))) | -n <= x <= n } and uses symmetry
 * to obtain the total number of convex‑hull vertices.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

typedef struct { int64_t x; int64_t y; } point_t;

/* cross product (b - a) × (c - a) */
static int64_t cross(const point_t *a, const point_t *b, const point_t *c)
{
    return (b->x - a->x) * (c->y - a->y) - (b->y - a->y) * (c->x - a->x);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* number of points in the upper boundary set */
    size_t len = (size_t)(2 * n + 1);

    /* allocate arrays for points and convex-hull stack */
    point_t *pts  = (point_t *)malloc(len * sizeof(point_t));
    point_t *stk  = (point_t *)malloc(len * sizeof(point_t));
    if (!pts || !stk) {
        free(pts);
        free(stk);
        return (seq_t)0;   /* allocation failure – should not happen */
    }

    /* compute point set (x, floor(sqrt(n^2 - x^2))) for x = -n .. n */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    for (size_t i = 0; i < len; ++i) {
        int64_t x = -(int64_t)n + (int64_t)i;   /* -n <= x <= n */
        seq_t  t   = n_sq - (seq_t)x * (seq_t)x;
        int64_t y  = (int64_t)floor_sqrt(t);
        pts[i].x   = x;
        pts[i].y   = y;
    }

    /* construct upper hull (right → left) with Andrew's monotone chain */
    size_t sz = 0;
    for (int64_t i = (int64_t)len - 1; i >= 0; --i) {
        point_t *p = &pts[i];
        while (sz >= 2 &&
               cross(&stk[sz - 2], &stk[sz - 1], p) <= 0)
            --sz;
        stk[sz++] = *p;
    }

    size_t upper_size = sz;

    /* The full convex hull has 2*upper_size-2 vertices (exclude the
     * duplicate points at the ends of the upper and lower chains). */
    seq_t result = (seq_t)(2 * upper_size - 2);

    free(pts);
    free(stk);
    return result;
}
