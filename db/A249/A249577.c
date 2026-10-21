/*
 * A249577  List of triples (r,s,t): the matrix M = [[1,4,4][1,3,2][1,2,1]] is
 *          raised to successive negative powers, then (r,s,t) are the square
 *          roots of M[3,1], M[1,1], M[1,3] respectively.
 * https://oeis.org/A249577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, easy
 * Formula:   a(n) = -2*a(n-3)+a(n-6); Initial values: a(0)=2, a(1)=-1,
 *            a(2)=1, a(3)=-4, a(4)=3, a(5)=-2
 * Range:     i8 n <= 14, i16 n <= 35, i32 n <= 71, i64 n <= 146,
 *            i128 n <= 299, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Cached implementation of the linear recurrence.  The cache is     */
/*  a static array that grows as needed and keeps all previously      */
/*  computed values so that the function works correctly for any      */
/*  order of calls.                                                  */
/* ------------------------------------------------------------------ */

static seq_t  *cache  = NULL; /* already computed terms: a[0] .. a[cache_size-1] */
static size_t cache_size = 0; /* number of elements currently computed   */
static size_t cache_cap  = 0; /* allocated capacity                      */

/* allocate or grow the cache to accommodate at least 'needed' entries */
static void ensure_cache(size_t needed) {
    if (needed <= cache_cap) return;          /* already big enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;

#ifdef __cplusplus
    seq_t *tmp = static_cast<seq_t *>(realloc(cache, new_cap * sizeof(seq_t)));
#else
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    /* Convert unsigned 64-bit index to an array index (size_t).   */
    size_t N = (size_t)n;

    /* Initialise the cache on the first call.                       */
    if (cache == NULL) {
        ensure_cache(6);                      /* at least 6 entries */
        cache[0] = (seq_t)  2;                /* a(0) */
        cache[1] = (seq_t) -1;                /* a(1) */
        cache[2] = (seq_t)  1;                /* a(2) */
        cache[3] = (seq_t) -4;                /* a(3) */
        cache[4] = (seq_t)  3;                /* a(4) */
        cache[5] = (seq_t) -2;                /* a(5) */
        cache_size = 6;
    }

    /* If the requested term is already computed, return it immediately. */
    if (N < cache_size) {
        return cache[N];
    }

    /* Ensure there is sufficient space for the new elements.           */
    ensure_cache(N + 1);

    /* Compute missing terms up to a(N) using the recurrence            */
    size_t start = cache_size;
    if (start < 6) start = 6;                 /* first index that needs computing */

    for (size_t i = start; i <= N; ++i) {
        /* a(i) = -2*a(i-3) + a(i-6) */
        cache[i] = -(seq_t)2 * cache[i - 3] + cache[i - 6];
    }

    cache_size = N + 1;                       /* all terms up to N available */
    return cache[N];
}
