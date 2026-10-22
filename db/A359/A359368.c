/*
 * A359368  Sequence begins 1, 1, 1; for even n > 3, a(n) = a(n/2 - 1) + a(n/2
 *          + 1); for odd n > 3, a(n) = -a((n-1)/2).
 * https://oeis.org/A359368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A359368
 *  a(1) = a(2) = a(3) = 1
 *  for n > 3,  if n is even:  a(n) = a(n/2 - 1) + a(n/2 + 1)
 *                  if n is odd :  a(n) = -a((n-1)/2)
 */
#include <stdlib.h>
#include <string.h>

/* static cache for memoised values */
static seq_t *cache = NULL;       /* 1‑based indexing, cache[0] unused   */
static idx_t cache_cap = 0;       /* current allocated size (>= 1)      */

/* enlarge cache to at least size n+1 */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= n + 1)
        return;

    /* double size or set to required size, whichever is larger */
    idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < n + 1)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failure – abort so caller sees failure */
        abort();
    }

    /* copy old data if any */
    if (cache) {
        memcpy(new_cache + 1, cache + 1, cache_cap * sizeof(seq_t));
        free(cache);
    }

    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1 – undefined for 0, return 0 */
        return 0;

    ensure_cache(n);

    /* compute all values up to n if not already computed */
    for (idx_t i = 1; i <= n; ++i) {
        if (i <= 3) {
            cache[i] = 1;
        } else if ((i & 1) == 0) {      /* even i */
            idx_t half = i >> 1;        /* i/2 */
            cache[i] = cache[half - 1] + cache[half + 1];
        } else {                       /* odd i */
            cache[i] = -(cache[(i - 1) >> 1]);   /* -a((i-1)/2) */
        }
    }

    return cache[n];
}
