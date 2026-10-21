/*
 * A267804  Binary representation of the n-th iteration of the "Rule 214"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10001*a(n-2) - 10000*a(n-4) initial values: a(0)=1,
 *            a(1)=111, a(2)=10111, a(3)=1100111 The function uses memoisation
 *            and grows a static cache
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* allocated size */
    static size_t computed = 0;      /* next index to compute */

    /* constants for the recurrence */
    static const seq_t C10001 = 10001;
    static const seq_t C10000 = 10000;

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 16;                      /* enough for the first few terms */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                 /* allocation failure fallback */

        cache[0] = 1;
        cache[1] = 111;
        cache[2] = 10111;
        cache[3] = 1100111;
        computed = 4;                         /* next index to compute */
    }

    /* enlarge cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;            /* allocation failure fallback */
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n (inclusive) */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        cache[i] = C10001 * cache[i-2] - C10000 * cache[i-4];
    }
    if ((size_t)n >= computed) computed = (size_t)n + 1;

    return cache[(size_t)n];
}
