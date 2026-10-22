/*
 * A331890  a(n) = -a(n-1) - a(n-2) + 2*a(n-3) with a(0)=3, a(1)=-1, a(2)=-1.
 * https://oeis.org/A331890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1) - a(n-2) + 2*a(n-3) with a(0)=3, a(1)=-1, a(2)=-1
 * Range:     i8 n <= 9, i16 n <= 22, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t last = 0;

    /* allocate initial cache for the first three terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory – unlikely in tests */
        cache[0] = 3;                  /* a(0) */
        cache[1] = -1;                 /* a(1) */
        cache[2] = -1;                 /* a(2) */
        last = 2;
    }

    /* if the desired index is already computed, just return it */
    if (n <= last) return cache[n];

    /* grow the cache to accommodate the new index */
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) return 0;          /* out of memory */
    cache = new_cache;

    /* compute missing terms using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = -cache[i - 1] - cache[i - 2] + 2 * cache[i - 3];
    }
    last = n;
    return cache[n];
}
