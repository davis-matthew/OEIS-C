/*
 * A331245  Triangles with integer sides i <= j <= k sorted by radius of
 *          enclosing circle, and, in case of ties, lexicographically by side
 *          lengths (smallest first). The sequence gives the middle side j.
 *          The other sides are in A331244 and A331246.
 * https://oeis.org/A331245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the middle side j of the nth triangle sorted by the
 *            radius of its enclosing circle (obtuse triangles use k/2). See
 *            OEIS A331245
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* internal struct for a triangle */
typedef struct {
    uint16_t i, j, k;   /* side lengths  i ≤ j ≤ k */
    uint64_t num, den;  /* squared radius = num / den  */
} Tri;

/* global state for memoisation */
static Tri *tris = NULL;          /* sorted array of triangles   */
static size_t tris_len = 0;       /* number of triangles stored */
static uint64_t current_S = 0;    /* maximum side length used   */

/* comparison function for qsort */
static int cmp_tri(const void *a_v, const void *b_v) {
    const Tri *a = (const Tri *)a_v;
    const Tri *b = (const Tri *)b_v;

    /* compare squared radii: num/den */
    __uint128_t left  = (__uint128_t)a->num * b->den;
    __uint128_t right = (__uint128_t)b->num * a->den;

    if (left < right) return -1;
    if (left > right) return 1;

    /* radii equal – lexicographic by side lengths */
    if (a->i != b->i) return a->i < b->i ? -1 : 1;
    if (a->j != b->j) return a->j < b->j ? -1 : 1;
    if (a->k != b->k) return a->k < b->k ? -1 : 1;
    return 0;
}

/* build triangles up to side length S and sort them */
static void build_triangles(uint64_t S) {
    if (tris) free(tris);

    /* rough capacity estimate: S^3 / 12  (triangles with i+j>k) */
    size_t est = (size_t)(S * S * S / 12) + 1000;
    tris = (Tri *)malloc(est * sizeof(Tri));
    tris_len = 0;

    for (uint64_t k = 1; k <= S; ++k) {
        for (uint64_t j = 1; j <= k; ++j) {
            uint64_t min_i = (k > j) ? (k - j + 1) : 1;
            for (uint64_t i = min_i; i <= j; ++i) {

                Tri t;
                t.i = (uint16_t)i;
                t.j = (uint16_t)j;
                t.k = (uint16_t)k;

                /* classification: right or obtuse */
                uint64_t lhs = i * i + j * j;
                uint64_t rhs = k * k;
                if (lhs <= rhs) {           /* not acute */
                    t.num = k * k;
                    t.den = 4;
                } else {                    /* acute triangle */
                    uint64_t prod = i * j * k;
                    t.num = prod * prod;

                    /* 16 * A^2 = (i+j+k)(-i+j+k)(i-j+k)(i+j-k) */
                    uint64_t t1 = i + j + k;
                    uint64_t t2 = j + k - i;  /* -i + j + k */
                    uint64_t t3 = k + i - j;  /* i - j + k */
                    uint64_t t4 = i + j - k;  /* i + j - k */
                    t.den = t1 * t2 * t3 * t4;
                }

                /* resize if necessary */
                if (tris_len >= est) {
                    est *= 2;
                    tris = (Tri *)realloc(tris, est * sizeof(Tri));
                }
                tris[tris_len++] = t;
            }
        }
    }

    qsort(tris, tris_len, sizeof(Tri), cmp_tri);
}

/* ensure we have at least n triangles pre‑computed */
static void ensure_precomputed(idx_t n) {
    if (n <= 0) return;
    if (tris_len >= (size_t)n) return;

    /* initialise or grow the side length bound */
    if (current_S == 0) current_S = 100;          /* start reasonable */
    while (tris_len < (size_t)n) {
        current_S *= 2;
        build_triangles(current_S);
    }
}

/* public function required by the build */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    ensure_precomputed(n);
    return (seq_t)tris[n - 1].j;
}
