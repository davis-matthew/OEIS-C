/*
 * A335348  Largest side of primitive triples for integer-sided triangles that
 *          have two perpendicular medians, the triples being ordered by
 *          increasing perimeter.
 * https://oeis.org/A335348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the largest side of the n‑th primitive * integer‑sided
 *            triangle with two perpendicular medians. * The sequence is
 *            generated from the two parametric families * described in the
 *            OEIS entry, sorted by increasing perimeter
 * Range:     u8 none, u16 none, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

/* ----------------------------------------------------------------------
 *  Integer utilities
 * ---------------------------------------------------------------------- */

/* ----------------------------------------------------------------------
 *  A vector of primitive triples (perimeter, largest side)
 * ---------------------------------------------------------------------- */
typedef struct {
    seq_t perim;
    seq_t largest;
} tri_t;

static tri_t *g_triples      = NULL;
static size_t g_triples_cnt  = 0;
static size_t g_triples_cap  = 0;

/* ----------------------------------------------------------------------
 *  Largest sides array (after sorting)
 * ---------------------------------------------------------------------- */
static seq_t *g_largest_array = NULL;
static size_t g_largest_cnt   = 0;

/* ----------------------------------------------------------------------
 *  Ensure enough capacity in the triples buffer
 * ---------------------------------------------------------------------- */
static void grow_triples(size_t needed)
{
    if (g_triples_cnt + needed <= g_triples_cap) return;
    size_t new_cap = g_triples_cap ? g_triples_cap : 64;
    while (g_triples_cnt + needed > new_cap) new_cap <<= 1;
    tri_t *tmp = (tri_t*)realloc(g_triples, new_cap * sizeof(tri_t));
    if (!tmp) abort();
    g_triples = tmp;
    g_triples_cap = new_cap;
}

/* ----------------------------------------------------------------------
 *  Multiply comparison for qsort
 * ---------------------------------------------------------------------- */
static int compare_tri(const void *p1, const void *p2)
{
    const tri_t *t1 = (const tri_t*)p1;
    const tri_t *t2 = (const tri_t*)p2;
    if (t1->perim > t2->perim) return 1;
    if (t1->perim < t2->perim) return -1;
    if (t1->largest > t2->largest) return 1;
    if (t1->largest < t2->largest) return -1;
    return 0;
}

/* ----------------------------------------------------------------------
 *  Generate the triples until a target number of terms is produced.
 *  The target is chosen moderately large to satisfy typical n ≤ 10000.
 * ---------------------------------------------------------------------- */
static void init_sequence(void)
{
    if (g_largest_cnt) return;            /* already done */

    const size_t TARGET = 20000;          /* enough for n ≈ 10000 */

    for (uint64_t u = 2; g_triples_cnt < TARGET; ++u) {
        /* ---------- first family: u/v > 3 ---------- */
        for (uint64_t v = 1; v <= (u - 1) / 3; ++v) {
            if (((u + v) & 1ULL) == 0) continue;          /* same parity */
            if (gcd_u64(u, v) != 1) continue;              /* coprime */
            if (((int64_t)u - 3 * (int64_t)v) % 5 == 0) continue; /* 5 | (u-3v) */

            uint64_t u_sq = u * u;
            uint64_t v_sq = v * v;
            uint64_t uv   = u * v;

            uint64_t a_raw = 2ULL * (u_sq - uv - v_sq);
            uint64_t b_raw = u_sq + 4ULL * uv - v_sq;
            uint64_t c     = u_sq + v_sq;

            if (a_raw == 0 || b_raw == 0) continue;
            uint64_t d = gcd_u64(a_raw, b_raw);
            d = gcd_u64(d, c);
            if (d != 1) continue;                      /* not primitive */

            seq_t largest = (a_raw > b_raw) ? (seq_t)a_raw : (seq_t)b_raw;
            seq_t perim   = (seq_t)a_raw + (seq_t)b_raw + (seq_t)c;

            grow_triples(1);
            g_triples[g_triples_cnt++] = (tri_t){perim, largest};
        }

        /* ---------- second family: 1 < u/v < 2 ---------- */
        for (uint64_t v = (u / 2) + 1; v < u; ++v) {
            if (((u + v) & 1ULL) == 0) continue;          /* same parity */
            if (gcd_u64(u, v) != 1) continue;              /* coprime */
            if (((int64_t)u - 2 * (int64_t)v) % 5 == 0) continue; /* 5 | (u-2v) */

            uint64_t u_sq = u * u;
            uint64_t v_sq = v * v;
            uint64_t uv   = u * v;

            uint64_t a_raw = 2ULL * (u_sq + uv - v_sq);
            uint64_t b_raw = -u_sq + 4ULL * uv + v_sq;
            uint64_t c     = u_sq + v_sq;

            if (a_raw == 0 || b_raw == 0) continue;
            uint64_t d = gcd_u64(a_raw, b_raw);
            d = gcd_u64(d, c);
            if (d != 1) continue;                      /* not primitive */

            seq_t largest = (a_raw > b_raw) ? (seq_t)a_raw : (seq_t)b_raw;
            seq_t perim   = (seq_t)a_raw + (seq_t)b_raw + (seq_t)c;

            grow_triples(1);
            g_triples[g_triples_cnt++] = (tri_t){perim, largest};
        }
    }

    /* Sort triples by perimeter (then largest side) */
    qsort(g_triples, g_triples_cnt, sizeof(tri_t), compare_tri);

    /* Store only the largest sides in order */
    g_largest_cnt = g_triples_cnt;
    g_largest_array = (seq_t*)malloc(g_largest_cnt * sizeof(seq_t));
    if (!g_largest_array) abort();
    for (size_t i = 0; i < g_largest_cnt; ++i)
        g_largest_array[i] = g_triples[i].largest;
}

/* ----------------------------------------------------------------------
 *  Public function required by the build
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* invalid index, but not used by OEIS */
    init_sequence();                /* ensure data ready */
    if ((idx_t)n > g_largest_cnt) return 0; /* out of generated range */
    return g_largest_array[n - 1];
}
