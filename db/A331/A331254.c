/*
 * A331254  Triangles with integer sides i <= j <= k sorted by radius of
 *          circumcircle, and, in case of ties, lexicographically by side
 *          lengths (smallest first). The sequence gives the shortest side i.
 *          The other sides are in A331255 and A331256.
 * https://oeis.org/A331254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Term: A331254 – shortest side i of integer triangles sorted by circumradius²
   ascending.  In case of equal R² the triangles are ordered lexicographically
   by (i,j,k).  The function a(n) returns the i‑value of the nth such triangle
   (n≥1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd64.h"

/* ---------- 128‑bit multiplication helper --------------------------------*/
#if defined(__SIZEOF_INT128__)  /* GCC, Clang, MSVC on 64‑bit */
typedef unsigned __int128 u128;
static inline void mul128(uint64_t a, uint64_t b, uint64_t *hi, uint64_t *lo)
{
    u128 prod = (u128)a * b;
    *lo = (uint64_t)prod;
    *hi = (uint64_t)(prod >> 64);
}
#else
/* Fallback implementation for compilers that lack native 128‑bit types. */
static inline void mul128(uint64_t a, uint64_t b, uint64_t *hi, uint64_t *lo)
{
    /* Split operands into 32‑bit halves */
    uint64_t a0 = a & 0xFFFFFFFFULL;
    uint64_t a1 = a >> 32;
    uint64_t b0 = b & 0xFFFFFFFFULL;
    uint64_t b1 = b >> 32;

    /* Partial products */
    uint64_t p00 = a0 * b0;        /* 64‑bit */
    uint64_t p01 = a0 * b1;
    uint64_t p10 = a1 * b0;
    uint64_t p11 = a1 * b1;

    /* Lower 64 bits */
    uint64_t lo = p00;
    uint64_t carry = 0;

    uint64_t upper = p01 + (p10 << 32);  /* may overflow 64 bits */
    if (upper < p01) carry++;

    lo += upper << 32;
    if (lo < upper << 32) carry++;

    /* High 64 bits */
    uint64_t hi = p11 + (p01 >> 32) + (p10 >> 32) + carry;

    *lo = lo;
    *hi = hi;
}
#endif

/* ---------- Utility: Euclidean GCD for 64‑bit values ---------------------*/

/* ---------- Triangle record ----------------------------------------------*/
typedef struct {
    uint32_t i, j, k;   /* side lengths,  i <= j <= k */
    uint64_t num, den;  /* reduced fraction R² = num / den  */
} triangle_t;

/* ---------- Global cache: sorted triangle list ----------------------------*/
static triangle_t *g_triangles = NULL;
static size_t       g_tri_count   = 0;
static uint32_t     g_max_side    = 0;

/* ---------- Comparator for qsort ------------------------------------------*/
static int compare_tri(const void *p1, const void *p2)
{
    const triangle_t *t1 = (const triangle_t *)p1;
    const triangle_t *t2 = (const triangle_t *)p2;

    /* Compare R²: t1.num/t1.den  <  t2.num/t2.den  ?
       Cross‑multiply using 128‑bit arithmetic.                       */
    uint64_t hi1, lo1, hi2, lo2;
    mul128(t1->num, t2->den, &hi1, &lo1);
    mul128(t2->num, t1->den, &hi2, &lo2);

    if (hi1 < hi2) return -1;
    if (hi1 > hi2) return  1;
    if (lo1 < lo2) return -1;
    if (lo1 > lo2) return  1;

    /* R² equal – tie‑break lexicographically by (i,j,k).             */
    if (t1->i != t2->i) return (t1->i < t2->i) ? -1 : 1;
    if (t1->j != t2->j) return (t1->j < t2->j) ? -1 : 1;
    if (t1->k != t2->k) return (t1->k < t2->k) ? -1 : 1;
    return 0;
}

/* ---------- Build all triangles with sides <= max_side -------------------*/
static void build_triangles(uint32_t max_side)
{
    /* First pass: count total number of triangles.   */
    size_t total = 0;
    for (uint32_t a = 1; a <= max_side; ++a) {
        for (uint32_t b = a; b <= max_side; ++b) {
            uint32_t c_min = b;
            uint32_t c_max = a + b - 1;
            if (c_max > max_side) c_max = max_side;
            if (c_min > c_max) continue;
            total += (size_t)(c_max - c_min + 1);
        }
    }

    /* Allocate array for triangles. */
    triangle_t *tri = (triangle_t *)malloc(total * sizeof(triangle_t));
    if (!tri) return;            /* allocation failed – unlikely at these sizes */

    /* Second pass: fill the array with triangle data. */
    size_t idx = 0;
    for (uint32_t a = 1; a <= max_side; ++a) {
        for (uint32_t b = a; b <= max_side; ++b) {
            uint32_t c_min = b;
            uint32_t c_max = a + b - 1;
            if (c_max > max_side) c_max = max_side;
            if (c_min > c_max) continue;
            for (uint32_t c = c_min; c <= c_max; ++c) {
                /* Compute circumradius squared R² = (abc)^2 / [16*s*(s-a)*(s-b)*(s-c)] .
                   Here we use reduced form num/den.                           */
                uint64_t s2   = (uint64_t)a + b + c;      /* 2*s  */
                uint64_t d1   = (uint64_t)b + c - a;      /* s-a */
                uint64_t d2   = (uint64_t)a + c - b;      /* s-b */
                uint64_t d3   = (uint64_t)a + b - c;      /* s-c */
                uint64_t den  = s2 * d1 * d2 * d3;        /* denominator of R²   */

                uint64_t a2 = (uint64_t)a * a;
                uint64_t b2 = (uint64_t)b * b;
                uint64_t c2 = (uint64_t)c * c;
                uint64_t num = a2 * b2 * c2;              /* numerator of R²     */

                uint64_t g = gcd64(num, den);
                num /= g;
                den /= g;

                tri[idx].i   = a;
                tri[idx].j   = b;
                tri[idx].k   = c;
                tri[idx].num = num;
                tri[idx].den = den;
                ++idx;
            }
        }
    }

    /* Sort triangles by R² and lexicographic tie‑breakers.            */
    qsort(tri, total, sizeof(triangle_t), compare_tri);

    /* Replace the global cache. */
    free(g_triangles);
    g_triangles = tri;
    g_tri_count = total;
    g_max_side  = max_side;
}

/* ---------- Ensure we have at least n triangles, enlarging bound if needed */
static void ensure_bound(idx_t n)
{
    if (g_tri_count >= (size_t)n)
        return;

    /* Initial bound if none yet. */
    uint32_t bound = g_max_side;
    if (bound == 0) bound = 10;

    /* Double until enough triangles are present. */
    while (g_tri_count < (size_t)n) {
        bound <<= 1;                     /* double bound */
        build_triangles(bound);
    }
}

/* ---------- Sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    /* n is 1‑based; sequence is defined for n ≥ 1. */
    ensure_bound(n);
    return (seq_t)g_triangles[n - 1].i;
}
