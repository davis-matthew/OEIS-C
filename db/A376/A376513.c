/*
 * A376513  Expansion of e.g.f. exp(x^3 * (1 + x)).
 * https://oeis.org/A376513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n-2) * (3*a(n-3) + 4*(n-3)*a(n-4)) for n >= 4 with
 *            initial values a(0) = 1, a(1) = 0, a(2) = 0, a(3) = 6 (The
 *            recurrence also works for n = 4,5,6,...)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses memoisation; it is safe for any order of calls. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache up to at least 4 entries */
    if (cache == NULL) {
        /* allocate enough space for the requested n or 4 entries */
        idx_t need = (n >= 4) ? n + 1 : 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * need);
        if (cache == NULL) exit(EXIT_FAILURE); /* allocation failure */

        /* initialise base values */
        cache[0] = (seq_t)1;
        if (need > 1) cache[1] = (seq_t)0;
        if (need > 2) cache[2] = (seq_t)0;
        if (need > 3) cache[3] = (seq_t)6;
        cache_size = need;

        if (n < cache_size) return cache[n];
    }

    /* grow cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (cache == NULL) exit(EXIT_FAILURE);
        cache_size = new_size;
    }

    /* compute missing values */
    idx_t start = 4;                     /* first term with recurrence */
    if (cache_size < 4) start = cache_size; /* really unlikely */
    for (idx_t i = start; i <= n; ++i) {
        seq_t term   = (seq_t)(i - 1) * (seq_t)(i - 2);
        seq_t inner  = (seq_t)3 * cache[i - 3]
                     + (seq_t)4 * (seq_t)(i - 3) * cache[i - 4];
        cache[i] = term * inner;
    }

    return cache[n];
}
