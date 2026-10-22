/*
 * A385242  Number of tilings of a 3 X n strip with dominos and U-shaped
 *          pentominos.
 * https://oeis.org/A385242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 2*a(n-5) initial values:
 *            a(0)=1, a(1)=0, a(2)=3, a(3)=0, a(4)=12, a(5)=2
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static const seq_t init[6] = {1, 0, 3, 0, 12, 2};

    /* If n is small, just return the stored value. */
    if (n < 6) return init[n];

    /* Allocate or grow the cache if necessary. */
    size_t need = (size_t)n + 1;
    if (cache == NULL) {
        cap = need > 64 ? need : 64;      /* start with a modest size */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 0; i < 6; ++i) cache[i] = init[i];
    } else if (need > cap) {
        size_t new_cap = cap;
        while (need > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = a(i-1) + 4*a(i-2) - 3*a(i-3) - 2*a(i-5) */
        seq_t term1 = cache[i-1] + (cache[i-2] << 2);
        seq_t term2 = (cache[i-3] << 1) + cache[i-3];          /* 3*a(i-3) */
        term2 = term2 + (cache[i-5] << 1);                     /* + 2*a(i-5) */
        cache[i] = term1 - term2;
    }

    return cache[n];
}
