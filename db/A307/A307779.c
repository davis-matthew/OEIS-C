/*
 * A307779  a(1) = 1; a(n+1) = Sum_{d|n, n/d odd} a(d).
 * https://oeis.org/A307779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = Sum_{d|n, n/d odd} a(d)
 * Range:     u8 n <= 57, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *a_cache = NULL;      /* dynamic array of known a(n) values      */
static size_t  cache_size = 0;     /* current allocated size (indices 0..size-1) */
static idx_t   last_computed = 1;  /* index of the last a(n) that has been computed */

/* Ensure the cache can hold values up to index n (inclusive). */
static void ensure_cache_size(idx_t n) {
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;          /* need indices 0..n      */
        seq_t *tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        /* copy old data */
        for (size_t i = 0; i < cache_size; ++i) tmp[i] = a_cache[i];
        /* initialize new slots to 0 */
        for (size_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
        free(a_cache);
        a_cache = tmp;
        cache_size = new_size;
    }
}

seq_t a(idx_t n) {
    /* Initialize the cache on first use. */
    if (a_cache == NULL) {
        cache_size = 2;                    /* indices 0 and 1 */
        a_cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        a_cache[0] = 0;
        a_cache[1] = 1;                    /* a(1) = 1 */
        last_computed = 1;
    }

    /* Make sure the array can be indexed up to n. */
    ensure_cache_size(n);

    /* Compute all terms from last_computed+1 up to n. */
    for (idx_t cur = last_computed + 1; cur <= n; ++cur) {
        idx_t m = cur - 1;                  /* n in the recurrence is cur-1 */
        seq_t sum = 0;
        for (idx_t d = 1; d <= m / d; ++d) {
            if (m % d == 0) {
                idx_t d1 = d;
                idx_t d2 = m / d;
                if ((d2 & 1ULL) != 0)         /* m/d1 odd -> d1 contributes?  */
                    sum += a_cache[d1];      /*   but m/d1 = d2             */
                if (d1 != d2 && ((d1 & 1ULL) != 0)) /* m/d2 = d1 odd ? */
                    sum += a_cache[d2];
            }
        }
        a_cache[cur] = sum;
    }

    last_computed = n;
    return a_cache[n];
}
