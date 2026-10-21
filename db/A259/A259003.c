/*
 * A259003  Number of (n+2)X(5+2) 0..1 arrays with every 3X3 subblock sum of
 *          the two medians of the central row and column plus the two sums of
 *          the diagonal and antidiagonal nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A259003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-12) - a(n-13) (n>23) Initial values
 *            (n=1..23): see OEIS entry A259003
 * Range:     u8 none, u16 none, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: 1‑based indexing, index 0 unused */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated array size (may exceed largest computed index) */
    static size_t last = 0;  /* largest n already computed */

    if (n == 0) return 0;                 /* offset starts at 1; a(0) is undefined */

    /* enlarge the array if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap * 2 : 32;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failed – caller may treat 0 as error */
        cache = tmp;
        cap   = new_cap;
    }

    /* if we have already computed up to n, just return the cached value */
    if (n <= last) return cache[n];

    /* initial terms (n=1..23) – hard‑coded seeds */
    static const seq_t init[24] = {
        0,
        (seq_t)113474, (seq_t)131557, (seq_t)397152, (seq_t)402742,
        (seq_t)219424, (seq_t)210946, (seq_t)35404,   (seq_t)30072,
        (seq_t)23070,  (seq_t)28125,  (seq_t)28868,  (seq_t)27162,
        (seq_t)31876,  (seq_t)24996,  (seq_t)25262,  (seq_t)23934,
        (seq_t)28976,  (seq_t)39602,  (seq_t)26248,  (seq_t)29360,
        (seq_t)24542,  (seq_t)29960,  (seq_t)30788
    };

    /* compute values from (last+1) up to n */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        if (i <= 23) {
            cache[i] = init[i];
        } else {
            cache[i] = cache[i-1] + cache[i-12] - cache[i-13];
        }
    }
    last = (size_t)n;
    return cache[n];
}
