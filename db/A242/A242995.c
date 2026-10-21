/*
 * A242995  a(n) = a(n-1)^2 + a(n-1)*a(n-2)^2 - a(n-2)^4 with a(1) = 2, a(2) =
 *          3.
 * https://oeis.org/A242995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1)^2 + a(n-1)*a(n-2)^2 - a(n-2)^4; a(1)=2, a(2)=3
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 6, i64 n <= 7, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage */
seq_t a(idx_t n) {
    /* Handle non‑positive indices defensively */
    if (n == 0) return (seq_t)0;

    /* Static cache: cache[0] unused, cache[1] = 2, cache[2] = 3 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity (indices) */
    static size_t cache_len = 0;   /* largest index stored (>=0) */

    /* Ensure base values are cached */
    if (cache_len < 2) {
        size_t needed = 2;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap : 1;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
            if (!tmp) return (seq_t)0;        /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache_len = 2;
    }

    /* If the requested index is already cached, return it */
    if ((size_t)n <= cache_len) return cache[(size_t)n];

    /* Expand cache capacity if necessary */
    size_t target = (size_t)n;
    if (cache_cap < target) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < target) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n, starting after the largest known index */
    for (size_t i = cache_len + 1; i <= target; ++i) {
        seq_t a_nm1 = cache[i - 1];
        seq_t a_nm2 = cache[i - 2];

        seq_t sq_nm1 = a_nm1 * a_nm1;   /* (a(n-1))^2 */
        seq_t sq_nm2 = a_nm2 * a_nm2;   /* (a(n-2))^2 */
        seq_t term3 = sq_nm2 * sq_nm2;   /* (a(n-2))^4 */

        cache[i] = sq_nm1 + a_nm1 * sq_nm2 - term3;
    }

    cache_len = target;
    return cache[target];
}
