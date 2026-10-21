/*
 * A249578  List of triples (r,s,t): the matrix M = [[4,12,9][2,7,6][1,4,4]]
 *          is raised to successive powers, then (r,s,t) are the square roots
 *          of M[3,1], M[1,1], M[1,3] respectively.
 * https://oeis.org/A249578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-3) - a(n-6) Initial values: a(0)=0, a(1)=1, a(2)=0,
 *            a(3)=1, a(4)=2, a(5)=3 The function memoizes previously computed
 *            values so calls can come in any order
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 52, u64 n <= 103,
 *            u128 n <= 204, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t types are provided by the build environment.
   seq_t must support +, -, and * operators on integer values. */
static seq_t *cache = NULL;          /* array of computed terms   */
static idx_t cache_len = 0;          /* number of terms currently stored   */
static size_t cache_cap = 0;         /* allocated capacity in elements      */

seq_t a(idx_t n)
{
    /* Ensure we have the base values in the cache. */
    if (cache == NULL) {
        cache_cap = 64;                           /* initial capacity   */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        /* base terms for n = 0 .. 5 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)3;
        cache_len = 6;
    }

    /* If the requested index is already computed, return it. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Grow the cache if needed. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap << 1 : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute new terms up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)(4 * cache[i - 3] - cache[i - 6]);
    }

    cache_len = n + 1;   /* update the number of terms stored */
    return cache[n];
}
