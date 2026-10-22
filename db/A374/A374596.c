/*
 * A374596  Floor(area) for the area of the largest square which fits in each
 *          Pythagorean triangle, with sides of the square on the legs of the
 *          triangle, for Pythagorean triangles ordered first by increasing
 *          perimeter, then shorter leg, then longer leg.
 * https://oeis.org/A374596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((x*y/(x+y))^2) where (x,y) are the legs of the nth
 *            * Pythagorean triangle ordered by increasing perimeter, then
 *            shorter leg, * then longer leg
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   compute_value, gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_value.h"
#include "../utils/gcd64.h"

/* Helper: Euclidean gcd for uint64_t (no external dependency). */

/* Structure holding a triangle and its evaluation. */
typedef struct {
    uint64_t perim;   /* perimeter */
    uint64_t a;       /* shorter leg (x) */
    uint64_t b;       /* longer leg  (y) */
} Triple;

/* Comparator for sorting by perimeter, then a, then b. */
static int cmp_triple(const void *p1, const void *p2)
{
    const Triple *t1 = (const Triple *)p1;
    const Triple *t2 = (const Triple *)p2;
    if (t1->perim < t2->perim) return -1;
    if (t1->perim > t2->perim) return 1;
    if (t1->a < t2->a) return -1;
    if (t1->a > t2->a) return 1;
    if (t1->b < t2->b) return -1;
    if (t1->b > t2->b) return 1;
    return 0;
}

/* Generate all Pythagorean triples with perimeter <= limit.  The array
 * must be freed by the caller. */
static void generate_triples(uint64_t limit, Triple **out, size_t *cnt)
{
    size_t cap = 1024;
    Triple *vec = (Triple *)malloc(cap * sizeof(Triple));
    size_t count = 0;

    for (uint64_t m = 2; ; m++) {
        if (2ULL * m * (m + 1ULL) > limit) break;

        for (uint64_t n = 1; n < m; n++) {
            if (((m - n) & 1ULL) == 0) continue;          /* same parity */
            if (gcd64(m, n) != 1) continue;               /* not coprime */

            uint64_t a0 = m * m - n * n;
            uint64_t b0 = 2ULL * m * n;
            if (a0 > b0) { uint64_t t = a0; a0 = b0; b0 = t; }
            uint64_t c0 = m * m + n * n;
            uint64_t perim0 = a0 + b0 + c0;
            if (perim0 > limit) continue;

            for (uint64_t k = 1; k * perim0 <= limit; k++) {
                uint64_t a = a0 * k;
                uint64_t b = b0 * k;
                uint64_t perim = perim0 * k;

                if (count == cap) {
                    cap <<= 1;
                    vec = (Triple *)realloc(vec, cap * sizeof(Triple));
                }
                vec[count].perim = perim;
                vec[count].a = a;
                vec[count].b = b;
                count++;
            }
        }
    }

    *out = vec;
    *cnt = count;
}

/* Build the cached sequence up to at least 'need' terms. */
static void build_cache(size_t need, seq_t **cache_ptr, size_t *len_ptr)
{
    /* Initial perimeter bound; will be doubled until enough triples are
     * generated. */
    uint64_t limit = 2000000;
    Triple *vec = NULL;
    size_t total = 0;

    while (1) {
        generate_triples(limit, &vec, &total);
        if (total >= need) break;
        free(vec);
        limit <<= 1;   /* double the bound and try again */
    }

    /* Allocate cache for all generated triples (safe even if > need). */
    seq_t *new_cache = (seq_t *)malloc(total * sizeof(seq_t));
    /* Sort triples to enforce OEIS ordering. */
    qsort(vec, total, sizeof(Triple), cmp_triple);
    for (size_t i = 0; i < total; i++) {
        new_cache[i] = compute_value(vec[i].a, vec[i].b);
    }

    free(vec);
    free(*cache_ptr);
    *cache_ptr = new_cache;
    *len_ptr = total;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n == 0) return 0;          /* undefined offset < 1 */
    size_t need = (size_t)n;

    if (need > cache_len) {
        build_cache(need, &cache, &cache_len);
    }
    return cache[n - 1];
}
