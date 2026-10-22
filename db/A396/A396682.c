/*
 * A396682  a(n) is twice the area of the convex hull of the cumulative
 *          prime-exponent coordinates of integers from 1 to n + 1.
 * https://oeis.org/A396682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = twice the area of the convex hull of all prime‑power
 *            points (p, e) with p^e ≤ n+1
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Use only headers that are available for C11 and C++17.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper type for a lattice point.  */
typedef struct { uint64_t x; uint64_t y; } point;

/* Signed cross product of (b-a) and (c-a).  */
static inline long long cross(const point *a,
                              const point *b,
                              const point *c)
{
    return (long long)(b->x - a->x) * (long long)(c->y - a->y)
         - (long long)(b->y - a->y) * (long long)(c->x - a->x);
}

/* Comparator: first by x, then by y.  */
static int cmp_point(const void *A, const void *B)
{
    const point *p = (const point *)A;
    const point *q = (const point *)B;
    if (p->x < q->x) return -1;
    if (p->x > q->x) return 1;
    if (p->y < q->y) return -1;
    if (p->y > q->y) return 1;
    return 0;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Work with the integer n, but we actually need points up to n+1. */
    size_t N = (size_t)n + 1;          /* fits because n is uint64_t */
    if (N < 2) return (seq_t)0;        /* no prime powers */

    /* Small prime sieve up to N. */
    bool *is_prime = (bool *)malloc((N + 1) * sizeof(bool));
    if (!is_prime) return (seq_t)0;
    for (size_t i = 0; i <= N; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= N; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= N; j += i)
                is_prime[j] = false;

    /* Collect all prime‑power points (p, e).  */
    point *pts = NULL;
    size_t pts_cap = 0, pts_len = 0;
    for (uint64_t p = 2; p <= N; ++p) {
        if (!is_prime[p]) continue;
        uint64_t e = 1;
        uint64_t power = p;
        while (power <= N) {
            if (pts_len == pts_cap) {
                size_t new_cap = pts_cap ? pts_cap * 2 : 16;
                point *tmp = (point *)realloc(pts, new_cap * sizeof(point));
                if (!tmp) { free(pts); free(is_prime); return (seq_t)0; }
                pts = tmp;
                pts_cap = new_cap;
            }
            pts[pts_len++] = (point){p, e};
            ++e;
            if (power > N / p) break;   /* avoid overflow */
            power *= p;
        }
    }
    free(is_prime);

    if (pts_len < 3) {                 /* less than 3 points → area 0 */
        free(pts);
        return (seq_t)0;
    }

    /* Sort points lexicographically. */
    qsort(pts, pts_len, sizeof(point), cmp_point);

    /* Build lower hull. */
    point *lower = (point *)malloc(pts_len * sizeof(point));
    size_t lower_len = 0;
    for (size_t i = 0; i < pts_len; ++i) {
        point p = pts[i];
        while (lower_len >= 2 &&
               cross(&lower[lower_len-2], &lower[lower_len-1], &p) <= 0)
            --lower_len;
        lower[lower_len++] = p;
    }

    /* Build upper hull. */
    point *upper = (point *)malloc(pts_len * sizeof(point));
    size_t upper_len = 0;
    for (size_t i = pts_len; i-- > 0; ) {
        point p = pts[i];
        while (upper_len >= 2 &&
               cross(&upper[upper_len-2], &upper[upper_len-1], &p) <= 0)
            --upper_len;
        upper[upper_len++] = p;
    }

    /* Combine hulls, omitting duplicate endpoints. */
    size_t hull_len = lower_len + upper_len - 2;
    point *hull = (point *)malloc(hull_len * sizeof(point));
    size_t h = 0;
    for (size_t i = 0; i < lower_len; ++i) hull[h++] = lower[i];
    for (size_t i = 1; i + 1 < upper_len; ++i) hull[h++] = upper[i];

    /* Shoelace formula for twice the signed area. */
    long long area = 0;
    for (size_t i = 0; i < hull_len; ++i) {
        size_t j = (i + 1) % hull_len;
        area += (long long)hull[i].x * hull[j].y
              - (long long)hull[j].x * hull[i].y;
    }
    if (area < 0) area = -area;

    /* Clean up temporary arrays. */
    free(pts);
    free(lower);
    free(upper);
    free(hull);

    return (seq_t)area;
}
