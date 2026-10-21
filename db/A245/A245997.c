/*
 * A245997  Number of length 2+2 0..n arrays with no pair in any consecutive
 *          three terms totalling exactly n.
 * https://oeis.org/A245997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A245997
   Number of length 2+2 0..n arrays with no pair in any consecutive
   three terms totalling exactly n.
   Formula (empirical recurrence, offset 1):
   a(n) = 3*a(n-1) - a(n-2) - 5*a(n-3) + 5*a(n-4) + a(n-5)
          - 3*a(n-6) + a(n-7)
   Initial values:
   a(1)=2, a(2)=12, a(3)=68, a(4)=208, a(5)=534,
   a(6)=1116, a(7)=2120.
   The function memoises results to allow arbitrary-order calls. */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* array of computed values, 1‑based */
    static idx_t  cache_len = 0;   /* largest index computed so far */
    static size_t cache_cap = 0;   /* capacity of the array */

    if (n <= 0) return (seq_t)0;   /* undefined for n==0, return 0 */

    /* For the first 7 terms we simply return the known constants. */
    if (n <= 7) {
        static const seq_t base[] = {
            (seq_t)2, (seq_t)12, (seq_t)68, (seq_t)208,
            (seq_t)534, (seq_t)1116, (seq_t)2120
        };
        return base[n-1];
    }

    /* Ensure the cache can hold up to index n. */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If this is the first time we need values beyond the 7th,
       initialise the cache with the base values. */
    if (cache_len == 0) {
        static const seq_t base[] = {
            (seq_t)2, (seq_t)12, (seq_t)68, (seq_t)208,
            (seq_t)534, (seq_t)1116, (seq_t)2120
        };
        for (idx_t i = 1; i <= 7; ++i) cache[i] = base[i-1];
        cache_len = 7;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t term  = (seq_t)3 * cache[i-1];           /* 3*a(n-1) */
        term -= cache[i-2];                            /* -a(n-2) */
        term -= (seq_t)5 * cache[i-3];                 /* -5*a(n-3) */
        term += (seq_t)5 * cache[i-4];                 /* +5*a(n-4) */
        term += cache[i-5];                            /* +a(n-5) */
        term -= (seq_t)3 * cache[i-6];                 /* -3*a(n-6) */
        term += cache[i-7];                            /* +a(n-7) */
        cache[i] = term;
    }

    cache_len = n;
    return cache[n];
}
