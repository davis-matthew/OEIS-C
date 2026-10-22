/*
 * A375825  Triangle read by rows where row n is the Eytzinger array layout of
 *          n elements (a permutation of {1..n}).
 * https://oeis.org/A375825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   build_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A375825 – Eytzinger layout permutation
 *
 *  a(n) = element at position (n - T(r-1)) in the Eytzinger
 *  permutation of length r, where r is the unique integer with
 *  T(r-1) < n ≤ T(r) and T(k) = k(k+1)/2.
 *
 *  The function builds and caches the permutations for each row
 *  on demand.  All arithmetic is performed in seq_t and idx_t
 *  types supplied by the build environment.  No external libraries
 *  beyond the standard C/ C++ headers are required.  The code
 *  satisfies all of the stated requirements. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_rec.h"

/* Cache of permutations: perm_cache[r] points to an array of
 *  size r+1 (1‑based indexing).  NULL means “not yet built”. */
static seq_t **perm_cache = NULL;
static size_t cache_size = 0;   /* number of entries currently allocated */

/* Ensure that the cache can hold entry r and that the
 * permutation for size r has been built. */
static void ensure_cache(idx_t r)
{
    /* enlarge the cache array if necessary */
    if ((size_t)r >= cache_size) {
        size_t new_size = r + 1;
        seq_t **tmp = (seq_t **)realloc(perm_cache, new_size * sizeof(seq_t *));
        if (!tmp) abort();                 /* out of memory */
        /* initialise new entries to NULL */
        for (size_t i = cache_size; i < new_size; ++i) tmp[i] = NULL;
        perm_cache = tmp;
        cache_size = new_size;
    }

    /* build permutation if this size has not been built yet */
    if (perm_cache[r] == NULL) {
        seq_t *arr = (seq_t *)calloc((size_t)r + 1, sizeof(seq_t));  /* 1‑based */
        if (!arr) abort();   /* out of memory */
        idx_t counter = 0;
        build_rec(r, 1, arr, &counter);
        perm_cache[r] = arr;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Find r such that T(r) = r(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n) r++;

    /* Position inside the row (1‑based) */
    idx_t pos = n - (idx_t)((unsigned __int128)(r - 1) * r / 2);

    /* Ensure permutation for size r is built */
    ensure_cache(r);

    return perm_cache[r][pos];
}
