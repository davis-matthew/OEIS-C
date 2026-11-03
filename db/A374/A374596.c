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
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   gcd_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
#include "../utils/mem.h"

/* Compute floor((x*y/(x+y))^2) using either 128‑bit intermediate (when
 * seq_t is a small unsigned type) or directly with seq_t for big
 * integer types. */
static seq_t compute_value(uint64_t x, uint64_t y)
{
    /* Branch on sizeof(seq_t) to avoid casting errors with big types. */
    if (sizeof(seq_t) <= 8) {          /* unsigned 8/16/32/64 bit */
        unsigned __int128 num = (unsigned __int128)x * x * y * y;
        unsigned __int128 den = (unsigned __int128)(x + y) * (x + y);
        return (seq_t)(num / den);
    } else {                           /* arbitrary-precision type */
        seq_t X = (seq_t)x;
        seq_t Y = (seq_t)y;
        seq_t num = X * X * Y * Y;
        seq_t den = (X + Y) * (X + Y);
        return num / den;
    }
}

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
            if (gcd_u64(m, n) != 1) continue;               /* not coprime */

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
    seq_t *new_cache = mem_new(seq_t, total);
    /* Sort triples to enforce OEIS ordering. */
    qsort(vec, total, sizeof(Triple), cmp_triple);
    for (size_t i = 0; i < total; i++) {
        new_cache[i] = compute_value(vec[i].a, vec[i].b);
    }

    free(vec);
    mem_free(*cache_ptr);
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
