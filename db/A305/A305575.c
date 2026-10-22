/*
 * A305575  List points (x,y) having integer coordinates, sorted first by
 *          radial coordinate r and in case of ties, by polar angle 0 <= phi <
 *          2*Pi in a polar coordinate system. Sequence gives x-coordinates.
 * https://oeis.org/A305575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 707, i16 n <= 9856, i32 n <= 9856, i64 n <= 9856,
 *            i128 n <= 9856, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A305575: the x‑coordinate of the n‑th integer lattice point
   (x,y) sorted by increasing radial distance r = sqrt(x²+y²);
   for equal r sort by polar angle 0 ≤ φ < 2π.
   Uses an incremental enumeration of all lattice points at each squared
   radius r², sorts them by angle, and caches the resulting x values. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root for 64‑bit unsigned integers: floor(sqrt(v)). */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t r = 0;
    for (int i = 31; i >= 0; --i) {
        idx_t test = r | ((idx_t)1 << i);
        if (test <= v / test)
            r = test;
    }
    return r;
}

struct _Point {
    seq_t x;
    seq_t y;
};

/* Comparator for qsort: order by polar angle, counter‑clockwise from
   the positive x‑axis. */
static int _point_cmp(const void *a, const void *b)
{
    const struct _Point *p = (const struct _Point *)a;
    const struct _Point *q = (const struct _Point *)b;

    /* Upper half‑plane comes before lower half‑plane.  Points on the
       positive x‑axis belong to the upper half‑plane. */
    bool up1 = (p->y > 0) || (p->y == 0 && p->x > 0);
    bool up2 = (q->y > 0) || (q->y == 0 && q->x > 0);
    if (up1 != up2)
        return up1 ? -1 : 1;

    /* For points in the same half‑plane, order by cross product
       sign, which is equivalent to comparing angles. */
    seq_t cross = (seq_t)p->x * q->y - (seq_t)p->y * q->x;
    if (cross > 0) return -1;
    if (cross < 0) return 1;
    return 0; /* identical point */
}

seq_t a(idx_t n)
{
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t *cache = NULL;
    static idx_t next_r2 = 0;   /* next squared radius to process */

    if (cache_len > (size_t)n)
        return cache[(size_t)n];

    while (cache_len <= (size_t)n) {
        idx_t r2 = next_r2;
        idx_t lim = isqrt_u64(r2);

        /* generate all lattice points (x,y) with x² + y² = r2 */
        size_t pt_cap = 8;
        size_t pt_len = 0;
        struct _Point *pts = (struct _Point *)malloc(pt_cap * sizeof(struct _Point));

        for (long long xc = -(long long)lim; xc <= (long long)lim; ++xc) {
            idx_t xabs = (xc >= 0) ? (idx_t)xc : (idx_t)(-xc);
            idx_t x2   = xabs * xabs;
            idx_t y2   = r2 - x2;
            if (y2 < 0) continue;

            if (y2 > 0) {
                idx_t y = isqrt_u64(y2);
                if (y * y == y2) {
                    /* (xc, y) and its reflection (xc, -y) */
                    if (pt_len == pt_cap) {
                        pt_cap <<= 1;
                        pts = (struct _Point *)realloc(pts, pt_cap * sizeof(struct _Point));
                    }
                    pts[pt_len++] = (struct _Point){ (seq_t)xc, (seq_t)y };
                    if (y != 0) {
                        if (pt_len == pt_cap) {
                            pt_cap <<= 1;
                            pts = (struct _Point *)realloc(pts, pt_cap * sizeof(struct _Point));
                        }
                        pts[pt_len++] = (struct _Point){ (seq_t)xc, (seq_t)(-(long long)y) };
                    }
                }
            } else { /* y2 == 0, point lies on the x‑axis */
                if (pt_len == pt_cap) {
                    pt_cap <<= 1;
                    pts = (struct _Point *)realloc(pts, pt_cap * sizeof(struct _Point));
                }
                pts[pt_len++] = (struct _Point){ (seq_t)xc, (seq_t)0 };
            }
        }

        /* sort the points by polar angle */
        qsort(pts, pt_len, sizeof(struct _Point), _point_cmp);

        /* append the sorted x‑values to the cache */
        size_t needed = cache_len + pt_len;
        if (needed > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
            while (new_cap < needed) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        for (size_t i = 0; i < pt_len; ++i)
            cache[cache_len++] = pts[i].x;

        free(pts);
        ++next_r2;
    }

    return cache[(size_t)n];
}
