/*
 * A368791  a(n) = A008544(n) * Sum_{k=0..n} 1/A008544(k).
 * https://oeis.org/A368791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n-1)*a(n-1)+1, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;  /* allocated capacity */
    static size_t len   = 0;  /* number of elements stored */

    /* grow the cache if necessary */
    if (len <= (size_t)n) {
        size_t need = (size_t)n + 1;
        if (cap < need) {
            /* allocate more space (double each time) */
            size_t new_cap = cap ? cap * 2 : 8;
            if (new_cap < need) new_cap = need;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cap   = new_cap;
        }

        /* initialise the cache */
        if (len == 0) {
            cache[0] = (seq_t)1;  /* a(0) = 1 */
            len = 1;
        }

        /* compute new entries using the recurrence */
        for (size_t i = len; i <= (size_t)n; ++i) {
            idx_t ii = (idx_t)i;
            seq_t factor = (seq_t)(3 * ii - 1);
            cache[i] = factor * cache[i - 1] + (seq_t)1;
        }
        len = (size_t)n + 1;
    }

    return cache[n];
}
