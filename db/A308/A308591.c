/*
 * A308591  Number of minimal total dominating sets in the (2n-1)-triangular
 *          snake graph.
 * https://oeis.org/A308591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + a(n-3) + a(n-4) + a(n-5) for n > 7 Initial
 *            values: a(1)=0, a(2)=3, a(3)=5, a(4)=7, a(5)=14, a(6)=22,
 *            a(7)=42
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 39, u64 n <= 79, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;       /* allocated size */
    static size_t cache_len = 0;       /* highest index with a computed value */

    if (n == 0) return (seq_t)0;       /* safety for n < 1 */

    /* initialise cache with first 8 values (index 0..7) */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t)); /* zero-initialised */
        /* index 0 unused, set to 0 for safety */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)7;
        cache[5] = (seq_t)14;
        cache[6] = (seq_t)22;
        cache[7] = (seq_t)42;
        cache_len = 7;
    }

    /* if the requested value is already computed, return it */
    if (n <= cache_len) {
        return cache[n];
    }

    /* ensure there is room for indices up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing values up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* recurrence applies only for i > 7; for i <=7 we already know values */
        if (i > 7) {
            cache[i] = (seq_t)2 * cache[i - 2] + cache[i - 3] + cache[i - 4] + cache[i - 5];
        }
    }

    cache_len = n;
    return cache[n];
}
