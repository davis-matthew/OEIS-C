/*
 * A275794  One half of the y members of the positive proper solutions (x =
 *          x1(n), y = y1(n)) of the first class for the Pell equation x^2 -
 *          2*y^2 = +7^2.
 * https://oeis.org/A275794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2); a(0) = 2, a(1) = 15
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation in caller-independent order */
    static seq_t  *cache = NULL;   /* pointer to array of seq_t */
    static size_t   cap   = 0;     /* current allocated capacity */
    static size_t   sz    = 0;     /* number of elements actually stored (>= 2 after init) */

    size_t N = (size_t)n;          /* cast index to size_t (safe for usual n) */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 2;
        sz  = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)15;
    }

    /* enlarge array if we need more space */
    if (N + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 4;
        while (newcap < N + 1) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap = newcap;
    }

    /* If already computed, return immediately */
    if (N < sz) return cache[N];

    /* Compute missing terms up to n using the recurrence */
    for (size_t i = sz; i <= N; ++i) {
        cache[i] = (seq_t)6 * cache[i-1] - cache[i-2];
    }

    sz = N + 1;
    return cache[N];
}
