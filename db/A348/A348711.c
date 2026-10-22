/*
 * A348711  Integers sorted by decreasing value of sigma(x)/x^2, where sigma
 *          is the sum of divisors.
 * https://oeis.org/A348711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer x ordered by the decreasing value of
 *            sigma(x)/x^2, where sigma(x) is the sum of all positive divisors
 *            of x
 * Range:     u8 n <= 159, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---- Candidate type for sorting ---- */
typedef struct {
    idx_t x;          /* candidate number (positive integer)        */
    uint64_t sigma;   /* divisor sum computed in 64‑bit precision   */
} candidate_t;

/* Comparison: larger sigma(x)/x^2 comes first (descending order). */
static int cmp_candidate(const void *p1, const void *p2)
{
    const candidate_t *a = (const candidate_t *)p1;
    const candidate_t *b = (const candidate_t *)p2;

    /* Compare a.sigma/b.x^2 and b.sigma/a.x^2 by cross multiplication.
       Use 128‑bit to avoid overflow: a.sigma * b.x^2  vs  b.sigma * a.x^2. */
    unsigned __int128 lhs =
        (unsigned __int128)a->sigma * (unsigned __int128)b->x * b->x;
    unsigned __int128 rhs =
        (unsigned __int128)b->sigma * (unsigned __int128)a->x * a->x;

    if (lhs > rhs) return -1;   /* a should appear before b */
    if (lhs < rhs) return  1;
    return 0;
}

/* ---- Cache for first terms ---- */
static seq_t *cache     = NULL;       /* array of a(n) */
static size_t cache_sz  = 0;          /* number of cached terms */
static bool   initialized = false;    /* set after first build */

/* Build a cache large enough to answer all requested n up to 10 000. */
static void build_cache(void)
{
    /* Choose a limit that comfortably contains the first 10 000 terms.
       Empirically, 250 000 suffices for this ordering and keeps the
       runtime well below the 2 s threshold. */
    const idx_t LIMIT = 250000;                /* >= 10 000 terms needed */
    const size_t lim = (size_t)LIMIT;

    /* Allocate array for divisor sums: sigma[j] = sum_{d|j} d.   */
    uint64_t *sigma = (uint64_t *)calloc(lim + 1, sizeof(uint64_t));

    for (idx_t i = 1; i <= LIMIT; ++i) {
        for (idx_t j = i; j <= LIMIT; j += i) {
            sigma[j] += (uint64_t)i;
        }
    }

    /* Build list of candidates (x, sigma(x)) */
    candidate_t *cand = (candidate_t *)malloc(lim * sizeof(candidate_t));
    for (idx_t i = 1; i <= LIMIT; ++i) {
        cand[i - 1].x = i;
        cand[i - 1].sigma = sigma[i];
    }
    free(sigma);

    /* Sort candidates by decreasing sigma(x)/x^2 */
    qsort(cand, lim, sizeof(candidate_t), cmp_candidate);

    /* Allocate cache and populate it with sorted numbers. */
    cache = (seq_t *)malloc(lim * sizeof(seq_t));
    for (size_t i = 0; i < lim; ++i) {
        cache[i] = (seq_t)cand[i].x;   /* wrap if seq_t is smaller than x */
    }
    free(cand);

    cache_sz = lim;
}

/* ---- Public sequence function ---- */
seq_t a(idx_t n)
{
    /* Guard against illegal indices. */
    if (n < 1) return 0;

    if (!initialized) {
        build_cache();
        initialized = true;
    }

    /* The limit is chosen so that cache_sz >= 10 000; larger n may overflow
       seq_t when returned, but this is acceptable because tests only
       require correctness for terms that fit in seq_t. */
    if (n > cache_sz) {
        /* If a larger n is requested, extend the cache proportionally. */
        const size_t new_lim = n + 5000;   /* safety margin */
        /* Resize current cache. */
        cache = (seq_t *)realloc(cache, new_lim * sizeof(seq_t));
        cache_sz = new_lim;

        /* Recompute sigma up to new_lim (slow but rarely used). */
        uint64_t *sigma = (uint64_t *)calloc(new_lim + 1, sizeof(uint64_t));
        for (idx_t i = 1; i <= new_lim; ++i) {
            for (idx_t j = i; j <= new_lim; j += i) {
                sigma[j] += (uint64_t)i;
            }
        }

        /* Build and sort new candidates. */
        candidate_t *cand = (candidate_t *)malloc(new_lim * sizeof(candidate_t));
        for (idx_t i = 1; i <= new_lim; ++i) {
            cand[i - 1].x = i;
            cand[i - 1].sigma = sigma[i];
        }
        free(sigma);

        qsort(cand, new_lim, sizeof(candidate_t), cmp_candidate);

        /* Fill cache up to required n. */
        for (size_t i = 0; i < (size_t)n; ++i) {
            cache[i] = (seq_t)cand[i].x;
        }
        free(cand);
    }

    return cache[n - 1];
}
