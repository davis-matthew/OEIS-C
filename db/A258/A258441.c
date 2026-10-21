/*
 * A258441  9-gonal numbers (A001106) that are the sum of two consecutive
 *          9-gonal numbers.
 * https://oeis.org/A258441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 39203*a(n-1) - 39203*a(n-2) + a(n-3) with a(1)=24486,
 *            a(2)=959892121, a(3)=37629690894906
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function returns the n‑th term of A258441, indexed from 1. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation  (indices 1..k are stored). */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of the cache array  */
    static idx_t  computed_max = 0;/* largest index already computed   */
    static int    initialized  = 0;/* 1 when the first three terms are set */

    /* Ensure the cache is large enough for indices up to n.
       Allocate at least 4 entries so that a(1) … a(3) can be stored. */
    size_t new_cap = (size_t)n + 1;
    if (new_cap < 4) new_cap = 4;
    if (cache_cap < new_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – unlikely in tests */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first three terms once.  */
    if (!initialized) {
        cache[1] = (seq_t)24486;
        cache[2] = (seq_t)959892121;
        cache[3] = (seq_t)37629690894906;
        computed_max = 3;
        initialized  = 1;
    }

    /* If the requested term is already known, return it. */
    if (n <= computed_max) {
        return cache[n];
    }

    /* Recurrence: a(i) = 39203*a(i-1) - 39203*a(i-2) + a(i-3).  */
    const seq_t C = (seq_t)39203;
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] = C * cache[i-1] - C * cache[i-2] + cache[i-3];
    }

    computed_max = n;
    return cache[n];
}
