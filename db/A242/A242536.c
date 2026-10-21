/*
 * A242536  Number of n-length words on {1,2,3,4} such that the maximal runs
 *          of identical odd integers are of odd length and the maximal runs
 *          of identical even integers are of even length.
 * https://oeis.org/A242536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A242536
 * Recurrence:
 *   a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 12
 *   a(n) = a(n-1) + 3*a(n-2) + 2*a(n-3) - 2*a(n-4)   (n ≥ 4)
 *
 * This implementation memoises all computed values so that calls in
 * arbitrary order are handled in O(1) per call after initialisation.
 * All arithmetic is performed using seq_t; no extra libraries are
 * required beyond <stdlib.h>.
 */

#include <stdlib.h>   /* for realloc, abort */

static seq_t *cache      = NULL;  /* memoisation storage */
static size_t cache_len  = 0;     /* number of terms currently stored */
static size_t cache_cap  = 0;     /* allocated capacity of 'cache' */

/* Ensure that cache holds terms up to index 'n' (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t target = (size_t)n + 1;
    if (cache_len >= target)
        return;                    /* nothing to do */

    /* Allocate or grow the cache if necessary. */
    if (cache_cap < target) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < target)
            new_cap = target;
        /* cast the void* returned by realloc for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failure – abort the program */
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the base terms if they have not yet been stored. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1) {
        cache[1] = (seq_t)2;
        cache_len = 2;
    }
    if (cache_len == 2) {
        cache[2] = (seq_t)4;
        cache_len = 3;
    }
    if (cache_len == 3) {
        cache[3] = (seq_t)12;
        cache_len = 4;
    }

    /* Compute any remaining terms up to 'n'. */
    for (size_t i = cache_len; i < target; ++i) {
        seq_t a1 = cache[i - 1];   /* a(n-1) */
        seq_t a2 = cache[i - 2];   /* a(n-2) */
        seq_t a3 = cache[i - 3];   /* a(n-3) */
        seq_t a4 = cache[i - 4];   /* a(n-4) */
        seq_t val = a1 + (seq_t)3 * a2 + (seq_t)2 * a3 - (seq_t)2 * a4;
        cache[i] = val;
    }
    cache_len = target;
}

/* Public sequence function (external linkage). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
