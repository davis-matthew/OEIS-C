/*
 * A334086  Positive numbers not of the form 2*x^4 + y*(y+1)/2 + z*(z+1)/2
 *          with x,y,z nonnegative integers.
 * https://oeis.org/A334086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 216, u64 n <= 216, u128 n <= 216,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the n-th positive integer that cannot be written as
   2*x^4 + Tri(y) + Tri(z) with non‑negative integers x,y,z,
   where Tri(k) = k*(k+1)/2.  This implementation searches
   candidate numbers in increasing order, using precomputation of
   all sums of two triangular numbers up to a dynamic limit.
   The sequence is cached so that repeated calls in any order
   are answered in O(1) after the initial construction. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* --------------  Globals -------------- */
static size_t repLimit = 0;                 /* current maximum value for precomputation */
static uint8_t *repTwoTri = NULL;           /* bitmap: repTwoTri[n] == 1  <=>  n is sum of two triangulars */
static seq_t shiftVals[128];                /* 2*x^4 values, up to 128 entries  */
static idx_t   numShifts = 0;               /* actual number of entries in shiftVals */

static seq_t *cache = NULL;                 /* cached a(n) values */
static size_t cacheSize = 0;                /* allocated capacity of cache */
static size_t cacheCount = 0;               /* number of cached terms */

/* --------------  Helper Functions -------------- */

/* Recompute repTwoTri bitmap up to newLimit.
   Also recompute shiftVals (2*x^4 values). */
static void recompute_rep(size_t newLimit)
{
    /* free old arrays */
    free(repTwoTri);

    /* allocate new bitmap */
    repTwoTri = (uint8_t *)calloc(newLimit + 1, 1);
    if (!repTwoTri) exit(EXIT_FAILURE);

    /* Build bitmap of sums of two triangular numbers */
    for (idx_t i = 0;; ++i) {
        seq_t t_i = (seq_t)i * (i + 1) / 2;
        if (t_i > newLimit) break;
        for (idx_t j = i;; ++j) {
            seq_t t_j = (seq_t)j * (j + 1) / 2;
            seq_t sum = t_i + t_j;
            if (sum > newLimit) break;
            repTwoTri[(size_t)sum] = 1;
        }
    }

    /* Compute shiftVals (2*x^4) up to newLimit */
    numShifts = 0;
    for (idx_t x = 0;; ++x) {
        seq_t x4 = (seq_t)x * x * x * x;         /* x^4  */
        seq_t shift = 2 * x4;                    /* 2*x^4 */
        if (shift > newLimit) break;
        if (numShifts < sizeof(shiftVals)/sizeof(shiftVals[0])) {
            shiftVals[numShifts++] = shift;
        }
    }

    repLimit = newLimit;
}

/* Ensure that the precomputation covers all values up to at least 'need'.
   If not, enlarge the limit by doubling until sufficient. */
static void ensure_rep_limit(idx_t need)
{
    if (need <= repLimit) return;
    size_t newLimit = repLimit ? repLimit : 1;
    while (newLimit < need) {
        newLimit *= 2;
    }
    recompute_rep(newLimit);
}

/* Add a new term to the cache, resizing the array if necessary. */
static void add_cache(seq_t val)
{
    if (cacheCount >= cacheSize) {
        size_t newSize = cacheSize ? cacheSize * 2 : 16;
        seq_t *newCache = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        if (!newCache) exit(EXIT_FAILURE);
        cache = newCache;
        cacheSize = newSize;
    }
    cache[cacheCount++] = val;
}

/* --------------  Main Function -------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, but safe */

    /* Ensure the cache has at least n terms */
    static idx_t lastChecked = 0;   /* largest candidate examined so far */

    /* Initialize precomputation if not yet done */
    if (repLimit == 0) recompute_rep(5000000);   /* start with 5 million */

    while (cacheCount < n) {
        /* Extend precomputation if needed */
        if (lastChecked + 1 > repLimit) ensure_rep_limit(lastChecked + 1);

        ++lastChecked;                 /* next candidate integer */

        /* Test whether lastChecked is representable */
        bool repr = false;
        seq_t candidate = (seq_t)lastChecked;

        for (idx_t i = 0; i < numShifts; ++i) {
            seq_t shift = shiftVals[i];
            if (shift > candidate) break;
            seq_t rem = candidate - shift;
            if (repTwoTri[(size_t)rem]) {
                repr = true;
                break;
            }
        }

        if (!repr) {                     /* non‑representable: new sequence term */
            add_cache(candidate);
        }
    }

    return cache[n - 1];
}
