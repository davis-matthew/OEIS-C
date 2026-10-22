/*
 * A331244  Triangles with integer sides i <= j <= k sorted by radius of
 *          enclosing circle, and, in case of ties, lexicographically by side
 *          lengths (smallest first). The sequence gives the shortest side i.
 *          The other sides are in A331245 and A331246.
 * https://oeis.org/A331244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A331244 – Shortest side of the nth integer triangle sorted by the
   radius of the smallest enclosing circle.

   For each triangle (i ≤ j ≤ k) let  
      • R² = (k²)/4   if i² + j² ≤ k²   (right or obtuse case, the
        smallest circle has diameter k),  
      • R² = (i² j² k²)/(s(s‑i)(s‑j)(s‑k))  otherwise, where 
        s = (i + j + k)/2.  This gives the circumradius squared.
   Triangles are ordered primarily by R² (ascending) and, for ties,
   lexicographically by (i, j, k).  a(n) is the i of the nth triangle
   in this order.  The routine below pre‑computes all triangles with
   k ≤ 500, which is sufficient to generate the first 10 000 terms
   (≈ 20 000 triangles).  Results are cached so repeated calls are O(1).
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* structure for a triangle together with its radius squared as a reduced fraction */
struct Tri {
    uint16_t i, j, k;   /* side lengths, i ≤ j ≤ k */
    uint64_t num, den;  /* R² = num / den  */
};

/* comparison for qsort: first by R², then lexicographically (i,j,k) */
static int tri_cmp(const void *p1, const void *p2)
{
    const struct Tri *a = (const struct Tri *)p1;
    const struct Tri *b = (const struct Tri *)p2;

    /* cross‑multiply to avoid loss of precision */
    unsigned __int128 left  = (unsigned __int128)a->num * b->den;
    unsigned __int128 right = (unsigned __int128)b->num * a->den;

    if (left < right) return -1;
    if (left > right) return  1;
    if (a->i < b->i)   return -1;
    if (a->i > b->i)   return  1;
    if (a->j < b->j)   return -1;
    if (a->j > b->j)   return  1;
    if (a->k < b->k)   return -1;
    if (a->k > b->k)   return  1;
    return 0;
}

/* dynamic array helper – expands capacity when needed */
static void push_tri(struct Tri **list, size_t *cnt, size_t *cap, const struct Tri *t)
{
    if (*cnt == *cap) {
        size_t newcap = (*cap == 0) ? 256 : (*cap << 1);
        struct Tri *tmp = (struct Tri *)realloc(*list, newcap * sizeof(**list));
        if (!tmp) abort();          /* out of memory */
        *list = tmp;
        *cap  = newcap;
    }
    (*list)[(*cnt)++] = *t;
}

/* global state */
static seq_t *a_seq  = NULL;   /* array of a(n) values, 0‑based index */
static size_t a_len  = 0;      /* number of terms stored */
static bool   ready  = false;

/* pre‑compute enough triangles to fill the first 10 000 terms */
static void precompute(void)
{
    if (ready) return;

    const uint32_t K_MAX = 500;   /* sufficient for n ≤ 10 000 */
    struct Tri *tri = NULL;
    size_t tri_cnt = 0, tri_cap = 0;

    for (uint32_t k = 1; k <= K_MAX; ++k) {
        /* j must be at least ceil(k/2) to satisfy i ≤ j ≤ k and i + j > k */
        uint32_t j_start = (k + 1) / 2;
        for (uint32_t j = j_start; j <= k; ++j) {
            /* i must be at least k - j + 1 (triangle inequality) */
            uint32_t i_min = k - j + 1;
            if (i_min < 1) i_min = 1;
            if (i_min > j) continue;
            for (uint32_t i = i_min; i <= j; ++i) {
                struct Tri t;
                t.i = (uint16_t)i;
                t.j = (uint16_t)j;
                t.k = (uint16_t)k;

                /* check for right or obtuse triangle */
                unsigned __int128 lhs = (unsigned __int128)i * i + (unsigned __int128)j * j;
                unsigned __int128 rhs = (unsigned __int128)k * k;
                if (lhs <= rhs) {
                    /* enclosing circle radius = k/2 */
                    t.num = (uint64_t)k * k;   /* numerator */
                    t.den = 4;                /* denominator */
                } else {
                    /* acute triangle – use circumcircle radius */
                    uint32_t s2   = i + j + k;          /* 2 * semiperimeter */
                    uint32_t t1   = s2 - 2 * i;
                    uint32_t t2   = s2 - 2 * j;
                    uint32_t t3   = s2 - 2 * k;
                    unsigned __int128 area2num = (unsigned __int128)s2 * t1 * t2 * t3;
                    if (area2num == 0) continue;       /* degenerate, skip */

                    unsigned __int128 rnum = (unsigned __int128)i * i *
                                            (unsigned __int128)j * j *
                                            (unsigned __int128)k * k;
                    t.num = (uint64_t)rnum;          /* numerator of R² */
                    t.den = (uint64_t)area2num;       /* denominator of R² */
                }

                push_tri(&tri, &tri_cnt, &tri_cap, &t);
            }
        }
    }

    /* sort all triangles */
    qsort(tri, tri_cnt, sizeof(struct Tri), tri_cmp);

    /* allocate sequence array and fill it with the i values */
    a_seq = (seq_t *)malloc(tri_cnt * sizeof(seq_t));
    if (!a_seq) abort();
    for (size_t idx = 0; idx < tri_cnt; ++idx) {
        a_seq[idx] = (seq_t)tri[idx].i;
    }
    a_len = tri_cnt;

    /* clean up */
    free(tri);
    ready = true;
}

/* required function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */
    precompute();
    if (n > a_len) return 0;       /* beyond pre‑computed range */
    return a_seq[n - 1];
}
