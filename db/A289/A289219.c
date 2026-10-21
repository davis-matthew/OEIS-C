/*
 * A289219  Areas of integer-sided triangles whose area equals 3 times their
 *          perimeter.
 * https://oeis.org/A289219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A289219 : Areas of integer‑sided triangles whose area equals
 *  3 times their perimeter.
 *  The sequence is finite.  This routine enumerates all solutions
 *  to the equation (y+z‑x)(x+z‑y)(x+y‑z) = 144·(x+y+z) with
 *  x,y,z positive integers, produces the corresponding areas
 *  A = 3·(x+y+z) and stores them in sorted order.
 *  Subsequent calls retrieve the nth term (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Comparator for qsort on idx_t values */
static int cmp_idx(const void *p1, const void *p2) {
    const idx_t a = *(const idx_t *)p1;
    const idx_t b = *(const idx_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Cached list of areas in ascending order */
static idx_t *cached_areas = NULL;
static size_t cached_count = 0;

/* Build all areas once.  The algorithm enumerates all positive integer
 * triples (u,v,w) with u ≤ v and (u*v-144) > 0.  Solving
 * w = 144·(u+v) / (u*v-144) gives all solutions of
 * u·v·w = 144·(u+v+w).  u,v,w must have the same parity to
 * ensure that the corresponding triangle sides are integers. */
static void build_sequence(void) {
    /* Upper bound sufficient for all known solutions */
    const idx_t U_MAX = 2000;

    idx_t *raw = NULL;
    size_t raw_len = 0, raw_cap = 0;

    for (idx_t u = 1; u <= U_MAX; ++u) {
        for (idx_t v = u; v <= U_MAX; ++v) {
            idx_t denom = u * v - 144;
            if (denom <= 0) continue;                  /* uv must exceed 144 */

            idx_t num = 144 * (u + v);
            if (num % denom != 0) continue;            /* w must be integer */

            idx_t w = num / denom;
            if (w < v) break;                          /* w decreases with larger v */

            /* Parity condition: u, v, w must be all even or all odd */
            if (((u ^ v) & 1) || ((v ^ w) & 1)) continue;

            idx_t area = 3 * (u + v + w);              /* 3·perimeter */

            /* Append to raw list */
            if (raw_len == raw_cap) {
                raw_cap = raw_cap ? raw_cap * 2 : 64;
                idx_t *tmp = (idx_t *)realloc(raw, raw_cap * sizeof(idx_t));
                if (!tmp) { free(raw); raw = NULL; return; }
                raw = tmp;
            }
            raw[raw_len++] = area;
        }
    }
    if (raw_len == 0) return;                          /* no solutions found */

    /* Sort and deduplicate */
    qsort(raw, raw_len, sizeof(idx_t), cmp_idx);

    size_t unique = 0;
    for (size_t i = 0; i < raw_len; ++i) {
        if (i == 0 || raw[i] != raw[i-1]) ++unique;
    }

    if (unique == 0) return;

    cached_areas = (idx_t *)malloc(unique * sizeof(idx_t));
    if (!cached_areas) { free(raw); raw = NULL; return; }
    cached_count = unique;

    size_t j = 0;
    for (size_t i = 0; i < raw_len; ++i) {
        if (i == 0 || raw[i] != raw[i-1]) {
            cached_areas[j++] = raw[i];
        }
    }
    free(raw);
}

/* Public sequence routine */
seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined for n < 1 */

    if (!cached_areas) build_sequence();  /* lazy initialisation */

    if (n > cached_count) return 0;      /* beyond known terms */

    return (seq_t)cached_areas[n-1];     /* cast to seq_t is safe */
}
