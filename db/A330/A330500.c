/*
 * A330500  a(n) = a(n-1) + a(floor(n/3)), a(1) = a(2) = 1.
 * https://oeis.org/A330500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(floor(n/3)), a(1)=a(2)=1 (offset 1)
 * Range:     u8 n <= 39, u16 n <= 201, u32 n <= 1981, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle offset: undefined behavior for n == 0, return 0 */
    if (n == 0) return (seq_t)0;

    /* Static cache to support out‑of‑order and repeated calls */
    static seq_t *cache = NULL;           /* 1‑based indexing, cache[0] unused   */
    static idx_t cache_cap = 0;           /* allocated size (max usable index) */
    static idx_t cache_computed = 0;      /* largest index whose value is valid */

    /* Allocate and initialise if first call */
    if (cache == NULL) {
        cache_cap = 64; /* arbitrary initial size */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed */

        /* Base values */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_computed = 2;
    }

    /* Ensure the cache can hold indices up to n */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < n) new_cap <<= 1; /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i / 3];
    }
    cache_computed = n;

    return cache[n];
}
