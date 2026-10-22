/*
 * A342562  List points (x,y,z) having integer coordinates, sorted first by
 *          R=x^2+y^2+z^2 and in case of ties, then by z and last by polar
 *          angle 0 <= phi < 2*Pi in a polar coordinate system. Sequence gives
 *          y-coordinates.
 * https://oeis.org/A342562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 1905, i16 n <= 9160, i32 n <= 9160, i64 n <= 9160,
 *            i128 n <= 9160, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A342562: a(n) is the y‑coordinate of the
 * integer lattice point (x,y,z) with
 *    R = x² + y² + z²
 * sorted first by R, then by z, then by polar angle φ = atan2(y,x)
 * (0 ≤ φ < 2π).  This function returns the n‑th term of that sequence.
 */

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "../utils/floor_sqrt.h"

/* point in the X‑Y plane */
typedef struct {
    seq_t x;
    seq_t y;
} pt_t;

/* comparison for sorting by polar angle from the positive X‑axis,
   in the range [0,2π). */
static int cmp_pt(const void *ap, const void *bp)
{
    const pt_t *a = (const pt_t *)ap;
    const pt_t *b = (const pt_t *)bp;

    /* upper half‑plane (including +X axis) comes first */
    int up_a = (a->y > 0) || (a->y == 0 && a->x > 0);
    int up_b = (b->y > 0) || (b->y == 0 && b->x > 0);
    if (up_a != up_b)
        return up_a ? -1 : 1;

    /* same half‑plane: use cross product to decide */
    seq_t cross = a->x * b->y - b->x * a->y;
    if (cross > 0) return -1;
    if (cross < 0) return 1;
    return 0;            /* angles are equal – impossible for distinct points */
}

/* main sequence function */
seq_t a(idx_t n)
{
    idx_t target = n;
    idx_t count  = 0;

    for (idx_t R = 0; ; ++R) {
        seq_t sqrtR = floor_sqrt((seq_t)R);

        for (seq_t z = -(seq_t)sqrtR; z <= sqrtR; ++z) {
            seq_t rem = (seq_t)R - z * z;
            if (rem < 0) continue;

            seq_t sqrtRem = floor_sqrt(rem);

            /* allocate space for the points in this (R,z) slice */
            /* upper bound: twice (sqrtRem+1) + a few extra */
            size_t max_pts = (size_t)(sqrtRem + 1) * 2 + 4;
            pt_t *pts = (pt_t *)malloc(max_pts * sizeof(pt_t));
            assert(pts != NULL);

            size_t len = 0;

            for (seq_t y = -(seq_t)sqrtRem; y <= sqrtRem; ++y) {
                seq_t y2   = y * y;
                seq_t rest = rem - y2;
                if (rest < 0) continue;

                seq_t x = floor_sqrt(rest);
                if (x * x != rest) continue;

                ps :;                                   /* points will be added below */
                pts[len++] = (pt_t){x, y};
                if (x != 0)
                    pts[len++] = (pt_t){-x, y};
            }

            if (len > 1)
                qsort(pts, len, sizeof(pt_t), cmp_pt);

            for (size_t i = 0; i < len; ++i) {
                if (count == target) {
                    seq_t yval = pts[i].y;
                    free(pts);
                    return yval;
                }
                ++count;
            }

            free(pts);
        }
    }
}
