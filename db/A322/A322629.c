/*
 * A322629  For a nonnegative number m with decimal digits (d_1, ..., d_k),
 *          let s(m) be the area of the convex hull of the set of points { (i,
 *          d_i), i = 1..k }; a(n) = 2 * s(n).
 * https://oeis.org/A322629
 *
 * Offset:    100    (a(100) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2 * area of convex hull of the points (i, digit_i)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The convex hull is computed by the monotone chain algorithm.  Since the
 * x‑coordinates i increase monotonically, sorting is trivial.  The area
 * is then obtained by the shoelace formula; the required output is the
 * integer 2*area, i.e. the absolute value of the shoelace sum. */

/* The function returns 0 for n < 100, as the OEIS data section starts at
 * offset 100. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { int x; int y; } point_t;

/* Helper: orientation cross product (b - a) × (c - a) */
static inline int cross(const point_t *a, const point_t *b, const point_t *c)
{
    return (b->x - a->x) * (c->y - a->y) - (b->y - a->y) * (c->x - a->x);
}

seq_t a(idx_t n)
{
    if (n < 100u) return 0;

    /* decompose n into decimal digits (most significant first) */
    uint64_t tmp = (uint64_t)n;
    int digits[32];
    int len = 0;
    if (tmp == 0) { digits[len++] = 0; }
    else {
        while (tmp > 0) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
        /* reverse to most significant first */
        for (int i = 0; i < len/2; ++i) {
            int t = digits[i];
            digits[i] = digits[len-1-i];
            digits[len-1-i] = t;
        }
    }

    /* construct points */
    point_t pts[32];
    for (int i = 0; i < len; ++i) {
        pts[i].x = i + 1;               /* 1‑based index */
        pts[i].y = digits[i];
    }

    /* monotone chain: lower hull */
    point_t lower[32];
    int lower_cnt = 0;
    for (int i = 0; i < len; ++i) {
        while (lower_cnt >= 2 &&
               cross(&lower[lower_cnt-2], &lower[lower_cnt-1], &pts[i]) <= 0)
            --lower_cnt;
        lower[lower_cnt++] = pts[i];
    }

    /* upper hull */
    point_t upper[32];
    int upper_cnt = 0;
    for (int i = len-1; i >= 0; --i) {
        while (upper_cnt >= 2 &&
               cross(&upper[upper_cnt-2], &upper[upper_cnt-1], &pts[i]) <= 0)
            --upper_cnt;
        upper[upper_cnt++] = pts[i];
    }

    /* concatenate lower and upper to get full hull, omit last point of each
     * because it's duplicated */
    int hull_cnt = 0;
    for (int i = 0; i < lower_cnt; ++i)
        ; /* keep all lower points */
    for (int i = 0; i < lower_cnt; ++i)
        lower[i].x = lower[i].x, lower[i].y = lower[i].y; /* silence */
    int *hull_idx = NULL; /* we build hull array directly */
    point_t hull[64];
    for (int i = 0; i < lower_cnt; ++i)
        hull[hull_cnt++] = lower[i];
    for (int i = 0; i < upper_cnt; ++i)
        hull[hull_cnt++] = upper[i];
    /* Remove duplicates: last point of lower is first of upper */
    if (hull_cnt >= 2 &&
        hull[hull_cnt-1].x == hull[0].x && hull[hull_cnt-1].y == hull[0].y)
        --hull_cnt;

    if (hull_cnt < 3) return 0;  /* area zero */

    /* shoelace sum */
    int64_t sum = 0;
    for (int i = 0; i < hull_cnt; ++i) {
        const point_t *p = &hull[i];
        const point_t *q = &hull[(i+1)%hull_cnt];
        sum += (int64_t)p->x * q->y - (int64_t)q->x * p->y;
    }
    if (sum < 0) sum = -sum;

    return (seq_t)sum;
}
