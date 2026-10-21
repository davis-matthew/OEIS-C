/*
 * A256184  First of two variations by Per Nørgård of his "infinity sequence",
 *          cf. A004718: u(0) = 0; u(3*n) = -u(n); u(3*n+1) = u(n) - 2;
 *          u(3*n+2) = u(n) - 1.
 * https://oeis.org/A256184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   u(0)=0; u(3*n)= -u(n); u(3*n+1)= u(n)-2; u(3*n+2)= u(n)-1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A256184 by memoising the recurrence.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values; cache[i] holds u(i).  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache[0] if this is the first call.  */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) abort();           /* memory allocation failed */
        cache[0] = 0;                          /* u(0) = 0 */
        cache_size = 1;
    }

    /* if value already computed, return it immediately.  */
    if (n < cache_size) {
        return cache[n];
    }

    /* enlarge cache to size n+1, then fill new entries.  */
    idx_t old_size = cache_size;
    size_t new_size   = (size_t)(n + 1);

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) abort();                /* memory allocation failed */
    cache = tmp;
    cache_size = new_size;

    for (idx_t i = old_size; i < cache_size; ++i) {
        idx_t k = i / 3;
        idx_t r = i % 3;
        seq_t val;
        if (r == 0) {
            val = -cache[k];
        } else if (r == 1) {
            val = cache[k] - 2;
        } else { /* r == 2 */
            val = cache[k] - 1;
        }
        cache[i] = val;
    }

    return cache[n];
}
