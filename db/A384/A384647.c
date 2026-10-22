/*
 * A384647  Expansion of (1+3*x+x^2) / (1-x-5*x^2-2*x^3).
 * https://oeis.org/A384647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A384647
   Generating function: (1+3*x+x^2)/(1 - x - 5*x^2 - 2*x^3)
   Recurrence: a(n) = a(n-1) + 5*a(n-2) + 2*a(n-3)  for n ≥ 3
   Initial values: a(0)=1, a(1)=4, a(2)=10
   This implementation memoises the sequence so that calls in arbitrary
   order are answered quickly. */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* array holding computed terms   */
    static size_t cache_len = 0;     /* current number of stored terms */

    /* First call: initialise the cache with the first three terms. */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure – safe fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)10;
    }

    /* If the requested index is already known, return it immediately. */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* We have to compute values up to, and including, index n. */
    size_t old_len = cache_len;                /* length before resizing   */
    size_t new_len = (size_t)n + 1;            /* number of elements needed */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;                       /* allocation failure       */
    cache = tmp;
    cache_len = new_len;

    /* Compute the new terms using the recurrence relation. */
    for (size_t i = old_len; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 1] + (seq_t)5 * cache[i - 2] + (seq_t)2 * cache[i - 3];
    }

    return cache[(size_t)n];
}
