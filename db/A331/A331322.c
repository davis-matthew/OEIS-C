/*
 * A331322  a(n) = (3*n + 1)!/(n!)^3.
 * https://oeis.org/A331322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n + 1)!/(n!)^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (derived from the OEIS entry; two integer divisions are exact). */
/* a(0) = 1;  for n > 0:  a(n) = a(n-1) * (3n+1) * 3 * (3n‑1) / (n*n)   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;   /* array holding computed terms */
    static size_t  cap    = 0;     /* current allocated capacity */
    static size_t  cached = 0;     /* number of terms already computed */

    /* Grow the cache if it is not large enough. */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *p = NULL;
        if (cache == NULL)
            p = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (p == NULL) abort();
        cache = p;
        cap   = new_cap;
    }

    /* Initialize the first term on first entry. */
    if (cached == 0) {
        cache[0] = (seq_t)1;        /* a(0) = 1 */
        cached   = 1;
    }

    /* Compute any missing terms up to index n. */
    for (size_t i = cached; i <= (size_t)n; ++i) {
        seq_t k   = (seq_t)i;                 /* current index as seq_t */
        seq_t tmp = cache[i - 1];

        /* tmp = tmp * (3k + 1) / k */
        tmp = tmp * ( (seq_t)3 * k + (seq_t)1 );
        tmp = tmp / k;

        /* tmp = tmp * 3 / k   (cancelling the k from the 3k factor) */
        tmp = tmp * (seq_t)3;
        tmp = tmp / k;

        /* tmp = tmp * (3k - 1) */
        tmp = tmp * ( (seq_t)3 * k - (seq_t)1 );

        cache[i] = tmp;
    }

    cached = (size_t)n + 1;
    return cache[(size_t)n];
}
