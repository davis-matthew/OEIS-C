/*
 * A267774  Decimal representation of the n-th iteration of the "Rule 207"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=6, a(2)=29, a(3)=123
 *            a(n)=7*a(n-1)-14*a(n-2)+8*a(n-3) for n>3 (Rule 207 elementary
 *            cellular automaton started from a single ON cell)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t* cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* handle trivial cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)29;
    if (n == 3) return (seq_t)123;

    /* initialise cache if necessary */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)29;
        cache[3] = (seq_t)123;
        cache_len = 4;
    }

    /* grow cache if needed */
    if ((idx_t)cache_len <= n) {
        if (n + 1 > cache_cap) {
            size_t new_cap = n + 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();          /* handle allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute missing terms using recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = 7 * cache[i - 1] - 14 * cache[i - 2] + 8 * cache[i - 3];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
