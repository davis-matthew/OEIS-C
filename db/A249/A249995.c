/*
 * A249995  Expansion of 1/((1+2*x)*(1-3*x)*(1-4*x)).
 * https://oeis.org/A249995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 2*a(n-2) - 24*a(n-3), with a(0)=1, a(1)=5,
 *            a(2)=27 The function memoises computed values to support
 *            arbitrary call order
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated capacity */
    static size_t len = 0;          /* number of terms computed (highest index + 1) */

    /* Allocate or grow the cache */
    if (cache == NULL) {
        cap = (n + 1 > 16 ? n + 1 : 16);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);        /* allocation failure */

        if (n >= 0) cache[0] = (seq_t)1;
        if (n >= 1) cache[1] = (seq_t)5;
        if (n >= 2) cache[2] = (seq_t)27;
        len = (n >= 2 ? 3 : n + 1);
    } else if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap = new_cap;
    }

    /* Ensure initial values are present */
    if (len < 3) {
        if (len <= 0) cache[0] = (seq_t)1;
        if (len <= 1) cache[1] = (seq_t)5;
        if (len <= 2) cache[2] = (seq_t)27;
        len = 3;
    }

    /* Return cached value if computed */
    if ((size_t)n < len)
        return cache[n];

    /* Compute missing terms using the recurrence */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t val = (seq_t)5 * a1 + (seq_t)2 * a2 - (seq_t)24 * a3;
        cache[i] = val;
    }
    len = (size_t)n + 1;
    return cache[n];
}
