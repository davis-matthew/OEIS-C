/*
 * A287808  Number of septenary sequences of length n such that no two
 *          consecutive terms have distance 1.
 * https://oeis.org/A287808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1) - 8*a(n-2) - 6*a(n-3) + 6*a(n-4);
   Initial values: a(0)=1, a(1)=7, a(2)=37, a(3)=197, a(4)=1049 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;
    static idx_t  cached = 0;        /* number of elements currently cached */

    /* Initialize cache with first five terms if necessary */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 5);
        if (!cache) return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)37;
        cache[3] = (seq_t)197;
        cache[4] = (seq_t)1049;
        cached = 5;
    }

    /* If requested term is already cached, return it immediately */
    if (n < (idx_t)cached)
        return cache[(size_t)n];

    /* Expand cache to accommodate n */
    size_t newlen = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, newlen * sizeof(seq_t));
    if (!tmp) {
        free(cache);
        cache  = NULL;
        cached = 0;
        return (seq_t)0; /* allocation failed */
    }
    cache = tmp;

    /* Compute new terms up to n */
    for (size_t i = cached; i <= n; ++i) {
        if (i <= 4) continue;    /* initial terms already set */
        seq_t t1 = 7 * cache[i - 1];
        seq_t t2 = 8 * cache[i - 2];
        seq_t t3 = 6 * cache[i - 3];
        seq_t t4 = 6 * cache[i - 4];
        cache[i] = t1 + t4 - t2 - t3; /* recurrence */
    }

    cached = newlen;
    return cache[(size_t)n];
}
