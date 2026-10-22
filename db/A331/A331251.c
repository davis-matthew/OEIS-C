/*
 * A331251  Triangles with integer sides i <= j <= k sorted by area, and, in
 *          case of ties, lexicographically by side lengths (smallest first).
 *          The sequence gives shortest side i. The other sides are in A331252
 *          and A331253.
 * https://oeis.org/A331251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* triangles sorted by area:  a(n) = i (shortest side) of the n-th triangle.
   Triangles are ordered first by 16 × area² (which is an integer product
   of four side‑sum expressions), then lexicographically by (i,j,k). */

#include <stdlib.h>
#include <stdint.h>

/* The build supplies the exact types for idx_t and seq_t. */
/* No redeclarations of idx_t or seq_t are made. */
/* The function has external linkage as required. */

typedef struct {
    uint64_t d;   /* 16*area², used only for sorting */
    idx_t i, j, k;
} Triangle;

static Triangle *tris = NULL;          /* array of all triangles up to a bound */
static size_t   tris_len = 0;          /* number of triangles stored */
static size_t   tris_cap = 0;          /* capacity of the array */
static int      built = 0;             /* flag indicating that the array is ready */

/* Grow the triangle array to at least 'need' elements. */
static void grow_tris(size_t need)
{
    if (need <= tris_cap) return;
    size_t new_cap = tris_cap ? tris_cap * 2 : 128;
    while (new_cap < need) new_cap *= 2;
    tris = (Triangle *)realloc(tris, new_cap * sizeof(Triangle));
    tris_cap = new_cap;
}

/* Comparison function for qsort: by area, then by i,j,k. */
static int cmp_tri(const void *a, const void *b)
{
    const Triangle *x = (const Triangle *)a;
    const Triangle *y = (const Triangle *)b;
    if (x->d < y->d) return -1;
    if (x->d > y->d) return 1;
    if (x->i < y->i) return -1;
    if (x->i > y->i) return 1;
    if (x->j < y->j) return -1;
    if (x->j > y->j) return 1;
    if (x->k < y->k) return -1;
    if (x->k > y->k) return 1;
    return 0;
}

/* Build the list of all integer triangles with sides up to MAX_SIDE.
   The chosen bound is sufficient to obtain at least 10 000 terms. */
static void build_triangles(void)
{
    const idx_t MAX_SIDE = 200; /* 200 gives ~1.3 million triangles */

    /* Estimate upper bound of number of triangles (i≤j≤k). */
    size_t estimated = (size_t)MAX_SIDE * (MAX_SIDE + 1) * (MAX_SIDE + 2) / 6;
    grow_tris(estimated);

    for (idx_t i = 1; i <= MAX_SIDE; ++i) {
        for (idx_t j = i; j <= MAX_SIDE; ++j) {
            for (idx_t k = j; k <= MAX_SIDE; ++k) {
                if (i + j <= k) continue;          /* triangle inequality */
                uint64_t t1 = i + j + k;
                uint64_t t2 = j + k - i;
                uint64_t t3 = i + k - j;
                uint64_t t4 = i + j - k;
                uint64_t d  = t1 * t2 * t3 * t4;    /* 16*area² */

                if (tris_len == tris_cap) grow_tris(tris_len + 1);
                tris[tris_len++] = (Triangle){ d, i, j, k };
            }
        }
    }
    qsort(tris, tris_len, sizeof(Triangle), cmp_tri);
    built = 1;
}

seq_t a(idx_t n)
{
    if (!built) build_triangles();

    if (n == 0 || n > tris_len) return 0;   /* undefined index */
    return (seq_t)tris[n - 1].i;
}
