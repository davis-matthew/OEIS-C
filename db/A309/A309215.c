/*
 * A309215  a(0)=0; thereafter a(n) = a(n-1)+n if a(n-1) odd, otherwise a(n) =
 *          a(n-1)-n.
 * https://oeis.org/A309215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=0; for n>0, a(n)=a(n-1)+n if a(n-1) is odd, otherwise
 *            a(n)=a(n-1)-n
 * Range:     i8 n <= 126, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache holds a(0)...a(cache_size-1) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* already computed? */
    if (n < cache_size)
        return cache[n];

    /* Extend cache to hold a(0)..a(n) */
    idx_t new_size = n + 1;
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * new_size);
    } else {
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
    }
    if (cache == NULL)
        return (seq_t)0;  /* allocation failure: undefined behaviour otherwise */

    /* start is the index of the first term to compute */
    idx_t start = cache_size;

    /* if this is the first call, initialise a(0) */
    if (start == 0) {
        cache[0] = (seq_t)0;
        start = 1;
        cache_size = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = start; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        if ((prev & (seq_t)1) != 0)       /* odd previous term */
            cache[i] = prev + (seq_t)i;
        else                             /* even previous term */
            cache[i] = prev - (seq_t)i;
    }

    cache_size = new_size;
    return cache[n];
}
