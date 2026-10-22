/*
 * A351929  Expansion of e.g.f. exp(x - x^3/6).
 * https://oeis.org/A351929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) - binomial(n-1,2) * a(n-3) a(0) = 1, a(1) = 1,
 *            a(2) = 1 (OEIS A351929)
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 19, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy memoisation – the cache grows only as far as needed. */
    static seq_t *cache = NULL;    /* The sequence values.          */
    static size_t  cap   = 0;      /* Allocated capacity (in elements). */
    static idx_t   last  = (idx_t)-1; /* Largest index already computed. */

    /* Initialise cache with first three terms if empty. */
    if (!cache) {
        const size_t init_cap = 3;          /* need indices 0..2 */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failed – return 0 as a safety */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cap   = init_cap;
        last  = 2;
    }
    /* Ensure the cache is large enough for index n. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed – return 0 */
        cache = tmp;
        cap   = new_cap;
    }
    /* Compute any missing values up to n. */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t bin = ((seq_t)(i - 1) * (seq_t)(i - 2)) / 2; /* binomial(i-1,2) */
            cache[i] = cache[i - 1] - bin * cache[i - 3];
        }
        last = n;
    }
    return cache[(size_t)n];
}
