/*
 * A342561  List points (x,y,z) having integer coordinates, sorted first by
 *          R^2 = x^2 + y^2 + z^2 and in case of ties, then by z and last by
 *          polar angle 0 <= phi < 2*Pi in a polar coordinate system. Sequence
 *          gives x-coordinates.
 * https://oeis.org/A342561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9007, i16 n <= 9007, i32 n <= 9007, i64 n <= 9007,
 *            i128 n <= 9007, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A342561: x‑coordinate of the n-th integer lattice point
   sorted first by R² = x² + y² + z², then by z, then by polar
   angle φ = atan2(y, x) in [0, 2π). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Helper: integer square root for uint64_t ---------- */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n + 1;
    while (high - low > 1) {
        uint64_t mid = low + (high - low) / 2;
        if (mid <= n / mid)  /* mid*mid <= n, avoid overflow     */
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* ---------- Point representation in the xy‑plane ---------- */
typedef struct {
    int32_t x;
    int32_t y;
} Point;

/* ---------- Comparator: sort by polar angle φ in [0, 2π) ---------- */
static int cmp_phi(const void *a, const void *b)
{
    const Point *p1 = (const Point *)a;
    const Point *p2 = (const Point *)b;

    /* Determine if vector is in the upper half (0 <= φ <= π) */
    bool up1 = (p1->y > 0) || (p1->y == 0 && p1->x >= 0);
    bool up2 = (p2->y > 0) || (p2->y == 0 && p2->x >= 0);

    if (up1 != up2)
        return (up1 ? -1 : 1);          /* upper half before lower half */

    /* Cross product to compare angles: p1 × p2 > 0  => p1 < p2 */
    int64_t cross = (int64_t)p1->x * p2->y - (int64_t)p1->y * p2->x;
    if (cross > 0) return -1;
    if (cross < 0) return 1;

    /* Same angle – tie‑break by coordinates for determinism */
    if (p1->x != p2->x) return (p1->x < p2->x) ? -1 : 1;
    if (p1->y != p2->y) return (p1->y < p2->y) ? -1 : 1;
    return 0;
}

/* ---------- Main function to compute a(n) ---------- */
seq_t a(idx_t n)
{
    idx_t idx = 0;          /* current index in the ordered list */

    /* Static array used to collect points for a given z level */
    static Point *pts = NULL;
    static size_t pts_cap = 0;

    for (uint64_t R2 = 0; ; ++R2)          /* increasing R² */
    {
        uint64_t max_z = isqrt_u64(R2);

        for (int64_t z = -(int64_t)max_z; z <= (int64_t)max_z; ++z)   /* increasing z */
        {
            uint64_t rxy = R2 - (uint64_t)(z * z);
            uint64_t max_x = isqrt_u64(rxy);

            size_t pts_len = 0;            /* reset for this z level */

            /* Enumerate all (x, y) with x² + y² = rxy */
            for (int64_t x = -(int64_t)max_x; x <= (int64_t)max_x; ++x)
            {
                uint64_t sub = rxy - (uint64_t)(x * x);
                uint64_t y = isqrt_u64(sub);
                if (y * y != sub)          /* not a perfect square */
                    continue;

                /* Store (x, y) */
                if (pts_len == pts_cap) {
                    size_t new_cap = pts_cap ? pts_cap * 2 : 8;
                    pts = (Point *)realloc(pts, new_cap * sizeof(Point));
                    pts_cap = new_cap;
                }
                pts[pts_len++] = (Point){(int32_t)x, (int32_t)y};

                /* Store (x, -y) if distinct */
                if (y != 0) {
                    if (pts_len == pts_cap) {
                        size_t new_cap = pts_cap ? pts_cap * 2 : 8;
                        pts = (Point *)realloc(pts, new_cap * sizeof(Point));
                        pts_cap = new_cap;
                    }
                    pts[pts_len++] = (Point){(int32_t)x, (int32_t)(-y)};
                }
            }

            /* Sort points at this z level by polar angle */
            if (pts_len > 1)
                qsort(pts, pts_len, sizeof(Point), cmp_phi);

            /* Emit points in order until we hit the target index */
            for (size_t i = 0; i < pts_len; ++i, ++idx) {
                if (idx == n)
                    return (seq_t)pts[i].x;
            }
        }
    }
}
