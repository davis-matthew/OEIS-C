/*
 * A331255  Triangles with integer sides i <= j <= k sorted by radius of
 *          circumcircle, and, in case of ties, lexicographically by side
 *          lengths (smallest first). The sequence gives the middle side j.
 *          The other sides are in A331254 and A331256.
 * https://oeis.org/A331255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = middle side j of the n‑th triangle with integer sides * i
 *            <= j <= k sorted by increasing circumradius; ties broken
 *            lexicographically * by (i,j,k). Computation is done by
 *            enumerating all triangles up to a * fixed side limit (200),
 *            storing their circumradius as a fraction * (num/den) [R² = i...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint64_t i, j, k;
    __uint128_t num;   /* numerator of R²  */
    __uint128_t den;   /* denominator of R² */
} tri_t;

static int cmp_tri(const void *p1, const void *p2)
{
    const tri_t *t1 = (const tri_t *)p1;
    const tri_t *t2 = (const tri_t *)p2;
    __uint128_t lhs = t1->num * t2->den;
    __uint128_t rhs = t2->num * t1->den;
    if (lhs < rhs) return -1;
    if (lhs > rhs) return  1;
    /* same radius – lexicographic order on side lengths */
    if (t1->i != t2->i) return (t1->i < t2->i) ? -1 : 1;
    if (t1->j != t2->j) return (t1->j < t2->j) ? -1 : 1;
    return (t1->k < t2->k) ? -1 : 1;
}

static bool is_init = false;
static tri_t *tris = NULL;
static size_t tri_cnt = 0;
#define MAX_SIDE 200

static void init_triangles(void)
{
    size_t cap = (size_t)MAX_SIDE * MAX_SIDE * MAX_SIDE / 6 + 1;
    tris = (tri_t *)malloc(cap * sizeof(tris[0]));
    tri_cnt = 0;
    for (uint64_t a = 1; a <= MAX_SIDE; ++a) {
        for (uint64_t b = a; b <= MAX_SIDE; ++b) {
            uint64_t c_max = a + b - 1;
            if (c_max > MAX_SIDE) c_max = MAX_SIDE;
            for (uint64_t c = b; c <= c_max; ++c) {
                uint64_t P = a + b + c;
                uint64_t Q = -a + b + c;
                uint64_t R = a - b + c;
                uint64_t S = a + b - c;
                __uint128_t num = (__uint128_t)a * a *
                                 (__uint128_t)b * b *
                                 (__uint128_t)c * c;
                __uint128_t den = (__uint128_t)P * Q * R * S;
                tris[tri_cnt].i = a;
                tris[tri_cnt].j = b;
                tris[tri_cnt].k = c;
                tris[tri_cnt].num = num;
                tris[tri_cnt].den = den;
                ++tri_cnt;
            }
        }
    }
    qsort(tris, tri_cnt, sizeof(tris[0]), cmp_tri);
    is_init = true;
}

seq_t a(idx_t n)
{
    if (!is_init)
        init_triangles();
    if (n == 0 || (idx_t)tri_cnt < n)
        return 0;
    return (seq_t)tris[n - 1].j;
}
