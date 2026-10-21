/*
 * A247518  a(n) = a(n-1) * (11*a(n-1) - 16*a(n-2)) / (a(n-1) + 10*a(n-2))
 *          with a(1) = 1, a(2) = 2.
 * https://oeis.org/A247518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1) = 1, a(2) = 2 a(n) = a(n-1) * (11*a(n-1) - 16*a(n-2)) /
 *            (a(n-1) + 10*a(n-2)) For n > 5 one can also use the linear
 *            recurrence a(n) = 11*a(n-1) - 66*a(n-2) + 264*a(n-3) -
 *            704*a(n-4) + 1024*a(n-5)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 16, i64 n <= 33, i128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;   /* number of terms stored in cache (a(1) -> cache[0]) */
    static idx_t cache_cap = 0;  /* allocated capacity */

    /* Ensure cache exists and has at least the first five terms */
    if (cached_n == 0) {
        cache_cap = 10;                  /* start with some spare room */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* out-of-memory; unlikely in test harness */

        /* initial values a(1) .. a(5) */
        cache[0] = (seq_t)1;             /* a(1) */
        cache[1] = (seq_t)2;             /* a(2) */
        cache[2] = (seq_t)1;             /* a(3) */
        cache[3] = (seq_t)-1;            /* a(4) */
        cache[4] = (seq_t)3;             /* a(5) */
        cached_n = 5;
    }

    /* If required term is already cached, return it */
    if (n <= cached_n) {
        return cache[n - 1];
    }

    /* Grow cache to hold up to n terms */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* realloc failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute new terms using the linear recurrence (valid for n >= 6) */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        idx_t k = i - 1; /* zero‑based index for a(i) */
        seq_t t1 = cache[k - 1];          /* a(i-1) */
        seq_t t2 = cache[k - 2];          /* a(i-2) */
        seq_t t3 = cache[k - 3];          /* a(i-3) */
        seq_t t4 = cache[k - 4];          /* a(i-4) */
        seq_t t5 = cache[k - 5];          /* a(i-5) */

        seq_t term = 11 * t1 - 66 * t2 + 264 * t3 - 704 * t4 + 1024 * t5;
        cache[k] = term;
    }
    cached_n = n;
    return cache[n - 1];
}
